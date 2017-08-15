#include <iostream>
#include <functional>
#include <GReflection.h>
#include <Object.h>
#include <Node.h>
#include "GSerializer.h"
using namespace std;

namespace GFramework
{
	std::map<unsigned int, std::vector<Object*>> GSerializer::reference_seekers;
	std::map<unsigned int, std::vector<Object*>> GSerializer::reference_providers;
	std::mutex GSerializer::reference_seeker_mutex;
	std::mutex GSerializer::reference_provider_mutex;

	std::map<unsigned int, std::vector<NodeSharedPtr*>> GDeserializer::reference_seekers;
	std::map<unsigned int, Object*> GDeserializer::reference_providers;
	std::mutex GDeserializer::reference_seeker_mutex;
	std::mutex GDeserializer::reference_provider_mutex;

	template<typename T>
	void binary_write(ostream& stream, T& t)
	{
		stream.write((const char*)&t, sizeof(t));
	}

	template<typename T>
	void binary_write(ostream& stream, T t)
	{
		stream.write((const char*)&t, sizeof(t));
	}

	template<typename T>
	void binary_read(istream& stream, T& t)
	{
		stream.read((char*)&t, sizeof(t));
	}

	template<typename T>
	void text_write(ostream& stream, T& t)
	{
		stream << t ;
	}

	template<typename T>
	void text_read(istream& stream, T& t)
	{
		stream >> t;
	}

	GSerializer::GSerializer(std::ofstream& _stream):stream(_stream)
	{

	}

	GSerializer::~GSerializer()
	{

	}

	void GSerializer::addReferenceSeeker(unsigned int _object_id, Object* _seeking_object)
	{
		std::lock_guard<std::mutex> lk(reference_seeker_mutex);

		(reference_seekers[_object_id]).push_back(_seeking_object);
	}

	void GSerializer::addReferenceProviders(unsigned int _object_id, Object* _seeking_object)
	{
		std::lock_guard<std::mutex> lk(reference_provider_mutex);

		(reference_providers[_object_id]).push_back(_seeking_object);
	}
	
	GSerializer & GBinarySerializer::operator<<(Object &_obj)
	{
		return (*this) << &_obj;
	}

	GSerializer & GBinarySerializer::operator<<(Object *_obj)
	{
		GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		char len = 1+strlen(_obj->metaclassName());
		stream.write(&len, sizeof(len));
		stream.write(_obj->metaclassName(), len);

		int i = 0;
		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());
			//stream << "\tProperty: " << property_name << "=" << i++ << endl;
			//binary_write(stream, i++);
			p->writeBinaryValue(stream, _obj);
		}
		//stream.flush();
		return *this;
	}

	GSerializer & GTextSerializer::operator<<(Object &_obj)
	{
		return (*this) << &_obj;
	}

	GSerializer & GTextSerializer::operator<<(Object *_obj)
	{
		GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		text_write(stream, string(_obj->metaclassName()));
		int i = 0;
		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());
			//text_write(stream, property_name.append(": "));
			text_write(stream, "\n");
			p->writeASCIIValue(stream, _obj);
		}

		return *this;
	}


	GDeserializer::GDeserializer(std::ifstream& _stream) :stream(_stream)
	{

	}

	GDeserializer::~GDeserializer()
	{

	}

	void GDeserializer::resolveDependencies()
	{
		for (auto itr1 = reference_seekers.begin(); itr1 != reference_seekers.end(); ++itr1)
		{
			unsigned int object_id = itr1->first;
			Object* provider = nullptr;
			provider = reference_providers[object_id];
			if (provider != nullptr)
			{
				for (auto itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2)
				{
					NodeSharedPtr* pointer = *itr2;
					//TODO avoid this dynamic cast
					pointer->reset(dynamic_cast<Node*>(provider));
				}
			}
			else
			{
				cout << "Error: NO reference provider for object id :" << object_id << endl;
			}
		}
	
	}

	void GDeserializer::addReferenceSeeker(unsigned int _object_id, NodeSharedPtr* _seeking_object)
	{
		std::lock_guard<std::mutex> lk(reference_seeker_mutex);

		(reference_seekers[_object_id]).push_back(_seeking_object);
	}

	void GDeserializer::addReferenceProviders(unsigned int _object_id, Object* _providing_object)
	{
		std::lock_guard<std::mutex> lk(reference_provider_mutex);

		reference_providers[_object_id] = _providing_object;
	}

	GDeserializer & GBinaryDeSerializer::operator >> (Object **_obj)
	{
		char class_name[255];
		char len = 0;
		stream.read(&len, sizeof(len));
		stream.read(class_name, len);

		GMetaclass* m = GMetaclassList::instance().getMetaclass(class_name);
		*_obj = m->createInstance();

		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		int i = 0;
		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());
			p->readBinaryValue(stream, *_obj);
		}
		addReferenceProviders((*_obj)->getObjectId(), (*_obj));
		return *this;
	}

	GDeserializer & GTextDeSerializer::operator>>(Object **_obj)
	{
		string class_name;
		stream >> class_name;
		//text_write(stream, _obj);
		GMetaclass* m = GMetaclassList::instance().getMetaclass(class_name.c_str());
		*_obj = m->createInstance();

		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		int i = 0;
		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());
			p->readASCIIValue(stream, *_obj);
		}
		addReferenceProviders((*_obj)->getObjectId(), (*_obj));
		return *this;
	}
}
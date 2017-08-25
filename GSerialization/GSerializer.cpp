#include <iostream>
#include <functional>
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

	GSerializer::~GSerializer()
	{
		close();
	}

	void GSerializer::close()
	{
		if (stream.is_open())
		{
			stream.close();
		}
	}
	
	GBinarySerializer::~GBinarySerializer()
	{
		close();
	}

	bool GBinarySerializer::open(const char* filename)
	{
		stream.open(filename, ios_base::out | ios_base::binary);
		if (!stream.is_open())
		{
			return false;
		}

		return true;
	}

	bool GBinarySerializer::writeMetaProperty(const Object* _obj, GMetaproperty* property)
	{
		property->writeBinaryValue(stream, _obj);
		return true;
	}

	GSerializer & GBinarySerializer::operator<<(Object &_obj)
	{
		return (*this) << &_obj;
	}

	GSerializer & GBinarySerializer::operator<<(Object *_obj)
	{
		if (!stream.is_open())
			return *this;

		GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
		const char* classname = _obj->metaclassName();
		size_t len = strlen(classname) + 1;
		unsigned int version = m->getVersion();
		stream.write((const char*)&len, sizeof(len));
		stream.write(classname, len);
		stream.write((const char*)&version, sizeof(version));

		_obj->serialize(*this);
		return *this;
	}

	bool GTextSerializer::open(const char* filename)
	{
		stream.open(filename, ios_base::out);
		if (!stream.is_open())
		{
			return false;
		}

		return true;
	}

	GTextSerializer::~GTextSerializer()
	{
		close();
	}

	bool GTextSerializer::writeMetaProperty(const Object* _obj, GMetaproperty* property)
	{
		property->writeASCIIValue(stream, _obj);
		stream << endl;
		return true;
	}

	GSerializer & GTextSerializer::operator<<(Object &_obj)
	{
		return (*this) << &_obj;
	}

	GSerializer & GTextSerializer::operator<<(Object *_obj)
	{
		if (!stream.is_open())
			return *this;

		GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
		stream << string(_obj->metaclassName()) << endl;
		stream << m->getVersion() << endl;
		_obj->serialize(*this);
		return *this;
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
		reference_seekers.clear();
		reference_providers.clear();
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

	void GDeserializer::close()
	{
		if(!stream.is_open())
		{ 
			return;
		}
		resolveDependencies();
		stream.close();
	}

	GDeserializer::~GDeserializer()
	{
		close();
	}

	bool GBinaryDeSerializer::open(const char* filename)
	{
		stream.open(filename, ios_base::in);
		if (!stream.is_open())
		{
			return false;
		}

		return true;
	}

	bool GBinaryDeSerializer::readMetaProperty(Object* _obj, GMetaproperty* property)
	{
		property->readBinaryValue(stream, _obj);
		return true;
	}

	GDeserializer & GBinaryDeSerializer::operator >> (Object **_obj)
	{
		size_t len = 0;
		stream.read((char*)&len, sizeof(len));

		char* classname = new char[len];
		stream.read(classname, len);
		
		unsigned int version = 0;				
		stream.read((char*)&version, sizeof(version));

		GMetaclass* m = GMetaclassList::instance().getMetaclass(classname);
		*_obj = m->createInstance();
		(*_obj)->deserialize(*this, version);
		
		addReferenceProviders((*_obj)->getObjectId(), (*_obj));

		return *this;
	}

	GBinaryDeSerializer::~GBinaryDeSerializer()
	{
		close();
	}

	bool GTextDeSerializer::open(const char* filename)
	{
		stream.open(filename, ios_base::in);
		if (!stream.is_open())
		{
			return false;
		}

		return true;
	}

	bool GTextDeSerializer::readMetaProperty(Object* _obj, GMetaproperty* property)
	{
		property->readASCIIValue(stream, _obj);
		return true;
	}

	GDeserializer & GTextDeSerializer::operator>>(Object **_obj)
	{
		string class_name;
		stream >> class_name;

		unsigned int version = 0;
		stream >> version;

		GMetaclass* m = GMetaclassList::instance().getMetaclass(class_name.c_str());
		*_obj = m->createInstance();
		(*_obj)->deserialize(*this, version);

		addReferenceProviders((*_obj)->getObjectId(), (*_obj));

		return *this;
	}

	GTextDeSerializer::~GTextDeSerializer()
	{
		close();
	}
}

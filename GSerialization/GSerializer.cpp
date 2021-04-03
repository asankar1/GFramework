#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <GFramework/GVariant/GObject.h>
//#include <Sample/Node.h>
#include <GFramework/GReflection/GReflection.h>
#include <GFramework/GSerialization/GSerializer.h>
using namespace std;

namespace GFramework
{
	std::map<unsigned int, std::vector<GObject*>> GSerializer::reference_seekers;
	std::map<unsigned int, std::vector<GObject*>> GSerializer::reference_providers;
	std::mutex GSerializer::reference_seeker_mutex;
	std::mutex GSerializer::reference_provider_mutex;

	std::map<unsigned int, std::vector<GObject*>> GDeserializer::reference_seekers;
	std::map<unsigned int, GObject*> GDeserializer::reference_providers;
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

	bool GBinarySerializer::writeMetaProperty(const GObject* _obj, GMetaproperty* property)
	{
		property->writeBinaryValue(stream, _obj);
		return true;
	}

	GSerializer & GBinarySerializer::operator<<(GObject &_obj)
	{
		return (*this) << &_obj;
	}

	GSerializer & GBinarySerializer::operator<<(GObject *_obj)
	{
		if (!stream.is_open())
			return *this;

		GMetaclass* m = _obj->getMetaclass();
		string nmstr = m->getFullNamespace();
		//GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
		const char* classname = nmstr.c_str();
		size_t len = strlen(classname) + 1;
		unsigned int version = m->getVersion();
		stream.write((const char*)&len, sizeof(len));
		stream.write(classname, len);
		stream.write((const char*)&version, sizeof(version));

		_obj->serialize(*this);
		return *this;
	}

	GTextSerializer::GTextSerializer() : objectDelimiter("#---------------#")
	{

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

	bool GTextSerializer::writeMetaProperty(const GObject* _obj, GMetaproperty* property)
	{
		stream << property->getName() << ":";
		property->writeASCIIValue(stream, _obj);
		stream << endl;
		return true;
	}

	GSerializer & GTextSerializer::operator<<(GObject &_obj)
	{
		return (*this) << &_obj;
	}

	GSerializer & GTextSerializer::operator<<(GObject *_obj)
	{
		if (!stream.is_open())
			return *this;

		GMetaclass* m = _obj->getMetaclass();
		string nmstr = m->getFullNamespace();
		//GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
		//const char* classname = m->getName().c_str();
		stream << "class name:" << nmstr << endl;
		stream << "ObjectId:" << _obj->getObjectId() << endl;
		stream << "class version:" << m->getVersion() << endl;
		//_obj->serialize(*this);

		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());

			writeMetaProperty(_obj, p);
		}
		stream << objectDelimiter << endl;
		return *this;
	}

	void GDeserializer::setObject_id(GObject* _obj, uint32 id)
	{
		_obj->setObjectId(id);
		addReferenceProviders(id, _obj);
	}

	void GDeserializer::resolveDependencies()
	{
		//TODO
#if 0
		for (auto itr1 = reference_seekers.begin(); itr1 != reference_seekers.end(); ++itr1)
		{
			unsigned int object_id = itr1->first;
			GObject* provider = nullptr;
			provider = reference_providers[object_id];
			if (provider != nullptr)
			{
				for (auto itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2)
				{
					GObjectSharedPtr* pointer = *itr2;
					//TODO avoid this dynamic cast
					pointer->reset(dynamic_cast<GObject*>(provider));
				}
			}
			else
			{
				cout << "Error: NO reference provider for object id :" << object_id << endl;
			}
		}
		reference_seekers.clear();
		reference_providers.clear();
#endif
	}

	void GDeserializer::addReferenceSeeker(unsigned int _object_id, GObjectSharedPtr* _seeking_object)
	{
		//TODO
#if 0
		std::lock_guard<std::mutex> lk(reference_seeker_mutex);

		(reference_seekers[_object_id]).push_back(_seeking_object);
#endif
	}

	void GDeserializer::addReferenceProviders(unsigned int _object_id, GObject* _providing_object)
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

	bool GBinaryDeSerializer::readMetaProperty(GObject* _obj, GMetaproperty* property)
	{
		property->readBinaryValue(stream, _obj);
		return true;
	}

	GDeserializer & GBinaryDeSerializer::operator >> (GObject **_obj)
	{
		size_t len = 0;
		stream.read((char*)&len, sizeof(len));

		char* classname = new char[len];
		stream.read(classname, len);
		
		unsigned int version = 0;				
		stream.read((char*)&version, sizeof(version));

		std::vector < std::string > namespaces_class;
		GMetaNamespace* nm = &GMetaNamespaceList::_global();
		istringstream str_stream(classname);
		for (std::string nmstr; std::getline(str_stream, nmstr, ':');)
		{
			namespaces_class.push_back(nmstr);
		}

		for (int i = 0; i < namespaces_class.size() - 1; i++)
		{
			nm = nm->getMetaNamespace(namespaces_class[i].c_str());
		}

		//GMetaclass* m = GMetaclassList::instance().getMetaclass(classname);
		//TODO: store and load the namespace of the meta class
		//GMetaclass* m = GMetaNamespaceList::_global().getMetaclass(classname);
		GMetaclass* m = nm->getMetaclass(namespaces_class.back().c_str());
		*_obj = m->createInstance();
		(*_obj)->deserialize(*this, version);
		
		addReferenceProviders((*_obj)->getObjectId(), (*_obj));

		return *this;
	}

	GBinaryDeSerializer::~GBinaryDeSerializer()
	{
		close();
	}

	GTextDeSerializer::GTextDeSerializer() : objectDelimiter("#---------------#")
	{

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

	bool GTextDeSerializer::readMetaProperty(GObject* _obj, GMetaproperty* property)
	{
		property->readASCIIValue(stream, _obj);
		return true;
	}

	std::pair<string, string> GTextDeSerializer::parseProperty(string line)
	{
		auto pos = line.find(":");
		string key = line.substr(0, pos);
		string value = line.substr(pos + 1);
		return std::make_pair(key, value);
	}

	GDeserializer & GTextDeSerializer::operator>>(GObject **_obj)
	{
		string line;
		string class_name;
		do
		{
			std::getline(stream , line);
			auto pos = line.find("class name:");
			if (pos != string::npos)
			{
				class_name = line.substr(pos + strlen("class name:"));
				break;
			}
		} while (!stream.eof());

		if (class_name.empty())
		{
			return *this;
		}

		uint32 unique_id= 0;
		do
		{
			std::getline(stream, line);
			auto pos = line.find("ObjectId:");
			if (pos != string::npos)
			{
				line = line.substr(pos + strlen("ObjectId:"));
				std::istringstream(line) >> unique_id;
				break;
			}
		} while (!stream.eof());

		if (line.empty())
		{
			return *this;
		}

		unsigned int version = 0;
		do
		{
			std::getline(stream, line);
			auto pos = line.find("class version:");
			if (pos != string::npos)
			{
				line = line.substr(pos + strlen("class version:"));
				std::istringstream(line) >> version;
				break;
			}
		} while (!stream.eof());

		if (line.empty())
		{
			return *this;
		}

		if(!stream.eof())
		{
			std::vector < std::string > namespaces_class;
			GMetaNamespace* nm = &GMetaNamespaceList::_global();
			istringstream str_stream(class_name);
			for (std::string nmstr; std::getline(str_stream, nmstr, ':');)
			{
				namespaces_class.push_back(nmstr);
			}

			for (int i = 0; i < namespaces_class.size() - 1; i++)
			{
				nm = nm->getMetaNamespace(namespaces_class[i].c_str());
			}

			GMetaclass* metaclass = nm->getMetaclass(namespaces_class.back().c_str());
			*_obj = metaclass->createInstance();
			
			setObject_id(*_obj, unique_id);

			while (!stream.eof())
			{
				std::getline(stream, line);
				auto pos = line.find(objectDelimiter);
				if (pos != string::npos)
				{
					break;
				}
				else
				{
					std::pair<string, string> prop = parseProperty(line);
					if (!prop.first.empty())
					{
						auto metaprop = metaclass->getProperty(prop.first.c_str());
						if (metaprop != nullptr)
						{
							metaprop->readASCIIValue(std::istringstream(prop.second), *_obj);
						}
					}
				}
			}
		}

		//(*_obj)->deserialize(*this, version);

		addReferenceProviders((*_obj)->getObjectId(), (*_obj));

		return *this;
	}

	GTextDeSerializer::~GTextDeSerializer()
	{
		close();
	}
}

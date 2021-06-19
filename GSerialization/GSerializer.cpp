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
	std::map<unsigned int, std::vector<GPointerPropertyInterface*> > GSerializer::reference_seekers;
	std::map<unsigned int, std::vector<GObjectSharedPtr> > GSerializer::reference_providers;
	std::mutex GSerializer::reference_seeker_mutex;
	std::mutex GSerializer::reference_provider_mutex;

	std::map<unsigned int, std::vector<GPointerPropertyInterface*> > GDeserializer::reference_seekers;
	std::map<unsigned int, GObjectSharedPtr> GDeserializer::reference_providers;
	std::mutex GDeserializer::reference_seeker_mutex;
	std::mutex GDeserializer::reference_provider_mutex;

	GSerializer::~GSerializer()
	{
		
	}

	bool GSerializer::open(OStreamSharedPtr _stream)
	{
		if (!_stream->good())
		{
			return false;
		}
		stream = _stream;
		return true;
	}

	void GSerializer::close()
	{
		/*if (stream->is_open())
		{
			stream.close();
		}*/
		postProcessor.process(*this);
		stream->flush();
	}

	OStreamSharedPtr GSerializer::getStream()
	{
		return stream;
	}

	void GSerializer::PostProcessor::process(GSerializer& serializer)
	{
		isProcessing = true;
		for (auto p : reference_providers)
		{
			for (auto obj : p.second)
			{
				serializer << *(obj);
			}
			p.second.clear();
		}
		reference_providers.clear();

		if(childPP)
			childPP->process(serializer);

		isProcessing = false;
	}
	void GSerializer::PostProcessor::addPointer(const GPointerPropertyInterface* pointer_prop)
	{
		if (isProcessing)
		{
			if (childPP == nullptr)
			{
				childPP = new GSerializer::PostProcessor();
			}
			childPP->addPointer(pointer_prop);
		}
		else
		{
			auto itr = reference_providers.find(pointer_prop->getObjectId());
			if (itr == reference_providers.end())
			{
				reference_providers.insert({ pointer_prop->getObjectId(), std::vector<GObjectSharedPtr>({ pointer_prop->getGObjectPointer() }) });
			}
			else
			{
				itr->second.push_back(pointer_prop->getGObjectPointer());
			}
		}
	}

	GSerializer::PostProcessor::~PostProcessor()
	{
		if(childPP)
			delete childPP;
	}
#if 0 // text and binary serializer	
	GBinarySerializer::~GBinarySerializer()
	{
		close();
	}

	bool GBinarySerializer::open(OStreamSharedPtr _stream)
	{
		if (!_stream->good())
		{
			return false;
		}
		stream = _stream;
		return true;
	}

	bool GBinarySerializer::writeMetaProperty(const GObject* _obj, GMetaproperty* property)
	{
		property->writeBinaryValue(*this, _obj);
		return true;
	}
	/*
	GSerializer & GBinarySerializer::operator<<(GObject &_obj)
	{
		return (*this) << &_obj;
	}

	GSerializer & GBinarySerializer::operator<<(GObject *_obj)
	{
		if (!stream->good())
			return *this;

		GMetaclass* m = _obj->getMetaclass();
		string nmstr = m->getFullNamespace();
		//GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
		const char* classname = nmstr.c_str();
		size_t len = strlen(classname) + 1;
		unsigned int version = m->getVersion();
		stream->write((const char*)&len, sizeof(len));
		stream->write(classname, len);
		stream->write((const char*)&version, sizeof(version));

		_obj->serialize(*this);
		return *this;
	}
	*/
	GTextSerializer::GTextSerializer() : objectDelimiter("#---------------#")
	{

	}

	bool GTextSerializer::open(OStreamSharedPtr _stream)
	{
		if (!_stream->good())
		{
			return false;
		}
		stream = _stream;
		return true;
	}

	GTextSerializer::~GTextSerializer()
	{
		close();
	}

	bool GTextSerializer::writeMetaProperty(const GObject* _obj, GMetaproperty* property)
	{
		*stream << property->getName() << ":";
		property->writeASCIIValue(*this, _obj);
		*stream << endl;
		return true;
	}

	/*GSerializer & GTextSerializer::operator<<(GObject &_obj)
	{
		return (*this) << &_obj;
	}

	GSerializer & GTextSerializer::operator<<(GObject *_obj)
	{
		if (!stream->good())
			return *this;

		GMetaclass* m = _obj->getMetaclass();
		string nmstr = m->getFullNamespace();
		//GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
		//const char* classname = m->getName().c_str();
		*stream << "class name:" << nmstr << endl;
		*stream << "ObjectId:" << _obj->getObjectId() << endl;
		*stream << "class version:" << m->getVersion() << endl;
		//_obj->serialize(*this);

		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());

			writeMetaProperty(_obj, p);
		}
		*stream << objectDelimiter << endl;
		return *this;
	}*/
#endif
	GDeserializer& GDeserializer::operator>>(GObjectSharedPtr* _obj)
	{
		read(_obj);
		if (*_obj)
		{
			addReferenceProviders((*_obj)->getObjectId(), (*_obj));
		}
		/*auto itr = reference_providers.find((*_obj)->getObjectId());
		if (itr == reference_providers.end())
		{
			reference_providers.insert({ (*_obj)->getObjectId(), GObjectSharedPtr((*_obj)) });
		}*/
		return *this;
	}

	void GDeserializer::setObject_id(GObjectSharedPtr _obj, uint32 id)
	{
		_obj->setObjectId(id);
		//addReferenceProviders(id, _obj);
	}

	void GDeserializer::resolveDependencies()
	{
		//TODO
#if 1
		for (auto itr1 = reference_seekers.begin(); itr1 != reference_seekers.end(); ++itr1)
		{
			unsigned int object_id = itr1->first;
			auto provider = reference_providers[object_id];
			if (provider)
			{
				for (auto itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2)
				{
					auto pointer = *itr2;
					//TODO avoid this dynamic cast
					//pointer->reset(dynamic_cast<GObject*>(provider));
					pointer->setGObjectPointer(provider);
				}
			}
			else
			{
				cout << "Error: No reference provider for object id :" << object_id << endl;
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

	void GDeserializer::addReferenceProviders(unsigned int _object_id, GObjectSharedPtr _providing_object)
	{
		std::lock_guard<std::mutex> lk(reference_provider_mutex);
		if (reference_providers.find(_object_id) != reference_providers.end())
			return;

		GObjectSharedPtr ptr(_providing_object);
		reference_providers[_object_id] = ptr;
	}

	bool GDeserializer::open(IStreamSharedPtr _stream)
	{
		if (!_stream->good())
		{
			return false;
		}
		stream = _stream;
		return true;
	}

	IStreamSharedPtr GDeserializer::getStream()
	{
		return stream;
	}

	void GDeserializer::close()
	{
		while (stream->good())
		{
			GObjectSharedPtr heap_object = nullptr;
			(*this) >> &heap_object;
			if (heap_object)
			{
				addReferenceProviders(heap_object->getObjectId(), heap_object);
			}
			
		}
		resolveDependencies();
	}

	GDeserializer::~GDeserializer()
	{
		
	}
#if 0 //Text and binary desierializers
	bool GBinaryDeSerializer::open(IStreamSharedPtr _stream)
	{
		if (!_stream->good())
		{
			return false;
		}
		stream = _stream;
		return true;
	}

	bool GBinaryDeSerializer::readMetaProperty(GObjectSharedPtr _obj, GMetaproperty* property)
	{
		property->readBinaryValue(*this, _obj);
		return true;
	}
	/*
	GDeserializer & GBinaryDeSerializer::operator >> (GObject **_obj)
	{
		size_t len = 0;
		stream->read((char*)&len, sizeof(len));

		char* classname = new char[len];
		stream->read(classname, len);
		
		unsigned int version = 0;				
		stream->read((char*)&version, sizeof(version));

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
	}*/

	GBinaryDeSerializer::~GBinaryDeSerializer()
	{
		close();
	}

	GTextDeSerializer::GTextDeSerializer() : objectDelimiter("#---------------#")
	{

	}

	bool GTextDeSerializer::open(IStreamSharedPtr _stream)
	{
		if (!_stream->good())
		{
			return false;
		}
		stream = _stream;
		return true;
	}

	bool GTextDeSerializer::readMetaProperty(GObjectSharedPtr _obj, GMetaproperty* property)
	{
		property->readASCIIValue(*this, _obj);
		return true;
	}

	std::pair<string, string> GTextDeSerializer::parseProperty(string line)
	{
		auto pos = line.find(":");
		string key = line.substr(0, pos);
		string value = line.substr(pos + 1);
		return std::make_pair(key, value);
	}

	/*GDeserializer & GTextDeSerializer::operator>>(GObject **_obj)
	{
		string line;
		string class_name;
		do
		{
			std::getline(*stream , line);
			auto pos = line.find("class name:");
			if (pos != string::npos)
			{
				class_name = line.substr(pos + strlen("class name:"));
				break;
			}
		} while (stream->good());

		if (class_name.empty())
		{
			return *this;
		}

		uint32 unique_id= 0;
		do
		{
			std::getline(*stream, line);
			auto pos = line.find("ObjectId:");
			if (pos != string::npos)
			{
				line = line.substr(pos + strlen("ObjectId:"));
				std::istringstream(line) >> unique_id;
				break;
			}
		} while (stream->good());

		if (line.empty())
		{
			return *this;
		}

		unsigned int version = 0;
		do
		{
			std::getline(*stream, line);
			auto pos = line.find("class version:");
			if (pos != string::npos)
			{
				line = line.substr(pos + strlen("class version:"));
				std::istringstream(line) >> version;
				break;
			}
		} while (stream->good());

		if (line.empty())
		{
			return *this;
		}

		if(stream->good())
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

			while (stream->good())
			{
				std::getline(*stream, line);
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
	}*/

	GTextDeSerializer::~GTextDeSerializer()
	{
		close();
	}
#endif
}

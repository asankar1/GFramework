#include <sstream>
#include <GFramework/GVariant/GObject.h>
#include <GFramework/GReflection/GReflection.h>
#include <GFramework/GSerialization/GStringSerializer.h>

using namespace std;

namespace GFramework
{

GStringSerializer::GStringSerializer() : objectDelimiter("#---------------#")
{

}

GStringSerializer::~GStringSerializer()
{

}
GSerializer& GStringSerializer::write(const GObjectSharedPtr& _obj)
{

	if (!stream->good())
		return *this;

	GMetaclass* m = _obj->getMetaclass();
	string nmstr = m->getFullNamespace();
	//GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
	//const char* classname = m->getName().c_str();
	*stream << "class name:" << nmstr << endl;
	*stream << "GObjectId:" << _obj->getObjectId() << endl;
	*stream << "class version:" << m->getVersion() << endl;
	//_obj->serialize(*this);

	std::vector<std::string> p_list;
	m->getEditablePropertiesList(p_list);
	m->getPropertiesList(p_list);

	for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
		string property_name = *it;
		auto p = m->getProperty(property_name.c_str());

		writeMetaProperty(_obj.get(), p);
	}
	*stream << objectDelimiter << endl;
	return *this;
}

GSerializer& GStringSerializer::write(const GPropertyInterface& property)
{
	property.writeASCIIValue(*stream);
	return *this;
}

/*
GSerializer& GStringSerializer::operator<<(GObject& _obj)
{
	if (!stream->good())
		return *this;

	GMetaclass* m = _obj.getMetaclass();
	string nmstr = m->getFullNamespace();
	//GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
	//const char* classname = m->getName().c_str();
	*stream << "class name:" << nmstr << endl;
	*stream << "GObjectId:" << _obj.getObjectId() << endl;
	*stream << "class version:" << m->getVersion() << endl;
	//_obj->serialize(*this);

	std::vector<std::string> p_list;
	m->getEditablePropertiesList(p_list);
	m->getPropertiesList(p_list);

	for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
		string property_name = *it;
		auto p = m->getProperty(property_name.c_str());

		writeMetaProperty(&_obj, p);
	}
	*stream << objectDelimiter << endl;
	return *this;
}

GSerializer& GStringSerializer::operator<<(GObject* _obj)
{
	if (!stream->good())
		return *this;

	GMetaclass* m = _obj->getMetaclass();
	string nmstr = m->getFullNamespace();
	//GMetaclass* m = GMetaclassList::instance().getMetaclass(_obj->metaclassName());
	//const char* classname = m->getName().c_str();
	*stream << "class name:" << nmstr << endl;
	*stream << "GObjectPointerId:" << _obj->getObjectId() << endl;
	return *this;
}*/

bool GStringSerializer::writeMetaProperty(const GObject* _obj, GMetaproperty* property)
{
	*stream << property->getName() << ":";
	property->writeASCIIValue(*this, _obj);
	if (!stream->good())
	{
		if (stream->eof())
		{
			cerr << "End of file reached when writing metaproperty:" << property << endl;
		}
		if (stream->fail())
		{
			cerr << "Operation failed when writing metaproperty:" << property << endl;
		}
		if (stream->bad())
		{
			cerr << "stream corrupted when writing metaproperty:" << property << endl;
		}
		return false;
	}
	*stream << endl;
	return true;
}

GStringDeserializer::GStringDeserializer() : objectDelimiter("#---------------#")
{

}

GStringDeserializer::~GStringDeserializer()
{

}


bool GStringDeserializer::readMetaProperty(GObjectSharedPtr _obj, GMetaproperty* property)
{
	property->readASCIIValue(*this, _obj);
	return true;
}

std::pair<string, string> GStringDeserializer::parseProperty(string line)
{
	auto pos = line.find(":");
	string key = line.substr(0, pos);
	string value = line.substr(pos + 1);
	return std::make_pair(key, value);
}

std::string GStringDeserializer::findNextProperty(string line)
{
	auto pos = line.find(":");
	if (pos != string::npos)
	{
		string key = line.substr(0, pos);
		size_t l = line.length();
		int p1 = stream->tellg();
		stream->seekg(-(l - pos)-1, ios_base::cur);
		int p2 = stream->tellg();
		return key;
	}
	return "";
}

GDeserializer& GStringDeserializer::read(GPropertyInterface& property)
{
	property.readASCIIValue(*stream);
	return *this;
}

GDeserializer& GStringDeserializer::read(GObjectSharedPtr* _obj)
{
	string line;
	string class_name;
	do
	{
		std::getline(*stream, line);
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

	uint32 unique_id = 0;
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

	if (stream->good())
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
		if (!metaclass)
		{
			cerr << "Unable to find metaclass " << namespaces_class.back() << endl;
		}
		*_obj = metaclass->createInstance();

		setObject_id(*_obj, unique_id);

		while (stream->good())
		{
			std::getline(*stream, line, ':');
			//auto pos = line.find(objectDelimiter);
			if (line == objectDelimiter)
			{
				break;
			}
			else
			{
				//std::pair<string, string> prop = parseProperty(line);
				//string prop = findNextProperty(line);
				string prop = line;
				//if (!prop.first.empty())
				{
					auto metaprop = metaclass->getProperty(prop.c_str());
					if (metaprop != nullptr)
					{
						metaprop->readASCIIValue(*this, *_obj);
						if (!stream->good())
						{
							if (stream->eof())
							{
								cerr << "End of file reached when reading metaproperty:" << prop << endl;
							}
							if (stream->fail())
							{
								cerr << "Operation failed when reading metaproperty:" << prop << endl;
							}
							if (stream->bad())
							{
								cerr << "stream corrupted when reading metaproperty:" << prop << endl;
							}
							return *this;
						}
						else
						{
							*stream >> std::ws;//read the remaining until the new line to start fresh for the next line
						}
					}
				}
			}
		}
	}

	//(*_obj)->deserialize(*this, version);

	//addReferenceProviders((*_obj)->getObjectId(), (*_obj));

	return *this;
}

/*
GDeserializer& GStringDeserializer::operator>>(GObject** _obj)
{
	string line;
	string class_name;
	do
	{
		std::getline(*stream, line);
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

	uint32 unique_id = 0;
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

	if (stream->good())
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
}
*/
namespace GStringSerializerWriter
{
	/*template<>
	GStringSerializer& write<GObject>(GStringSerializer& stream, GObject& value)
	{
		//stream << &value;
		return stream;
	}*/

	template<>
	GStringSerializer& write<int>(GStringSerializer& stream, int& value)
	{
		*stream.getStream() << value << " ";
		return stream;
	}
}

namespace GStringDeserializerReader
{
	template<>
	GStringDeserializer& read<int>(GStringDeserializer& stream, int& value)
	{
		*stream.getStream() >> value;
		return stream;
	}
}

}

#include "GReflection.h"


namespace GFramework
{
	std::map<std::string, GMetaclass*> Gmetatable;

	GMetaclassList::GMetaclassList()
	{

	}

	GMetaclassList::~GMetaclassList()
	{

	}

	GReflection::GReflection()
	{
	}


	GReflection::~GReflection()
	{
	}

	void GMetaclass::addFunction(const char * _name, GMetafunction * _f)
	{
		Gmetafunctions.insert(std::make_pair(std::string(_name), _f));
	}

	void GMetaclass::addProperty(const char * _name, GMetaproperty * _p)
	{
		Gmetaproperties.insert(std::make_pair(std::string(_name), _p));
	}

	void GMetaclass::addEditableProperty(const char * _name, GMetaproperty * _p)
	{
		Gmetaeditableproperties.insert(std::make_pair(std::string(_name), _p));
	}

	GMetaclass::GMetaclass():name("")
	{

	}
}

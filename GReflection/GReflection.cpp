#include <GReflection/GReflection.h>


namespace GFramework
{
	std::map<std::string, GMetaclass*> Gmetatable;

	template<>
	void GMetafunction_Base::iterate_arg_type<void>(lua_State* L, std::vector<GVariant>& args, int offset) {

	}

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

	void GMetaclass::addFunction(const char * _name, GMetaMemberfunction * _f)
	{
		Gmetamemberfunctions.insert(std::make_pair(std::string(_name), _f));
	}

	void GMetaclass::addStaticFunction(const char * _name, GMetaStaticfunction * _f)
	{
		GmetaStaticfunctions.insert(std::make_pair(std::string(_name), _f));
	}	

	void GMetaclass::addProperty(const char * _name, GMetaproperty * _p)
	{
		Gmetaproperties[properties_version].insert(std::make_pair(std::string(_name), _p));
	}

	void GMetaclass::addEditableProperty(const char * _name, GMetaproperty * _p)
	{
		Gmetaeditableproperties[properties_version].insert(std::make_pair(std::string(_name), _p));
	}

	GMetaclass::GMetaclass():name("")
	{
		properties_version = 0;
	}
}

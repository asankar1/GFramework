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

	void GMetaclass::addConstructor(const char* _name, GMetaconstructor* _c)
	{
		assert(_name && "GMetaclass::addConstructor name must not be null");
		assert(_c && "GMetaclass::addConstructor GMetaconstructor must not be null");

		Gmetaconstructors.insert(std::make_pair(std::string(_name), _c));
	}

	void GMetaclass::addFunction(const char * _name, GMetaMemberfunction * _f)
	{
		assert(_name && "GMetaclass::addFunction name must not be null");
		assert(_f && "GMetaclass::addFunction GMetaMemberfunction must not be null");

		Gmetamemberfunctions.insert(std::make_pair(std::string(_name), _f));
	}

	void GMetaclass::addStaticFunction(const char * _name, GMetaStaticfunction * _f)
	{
		assert(_name && "GMetaclass::addStaticFunction name must not be null");
		assert(_f && "GMetaclass::addStaticFunction GMetaStaticfunction must not be null");

		GmetaStaticfunctions.insert(std::make_pair(std::string(_name), _f));
	}	

	void GMetaclass::addProperty(const char * _name, GMetaproperty * _p)
	{
		assert(_name && "GMetaclass::addProperty name must not be null");
		assert(_p && "GMetaclass::addProperty GMetaproperty must not be null");

		Gmetaproperties[properties_version].insert(std::make_pair(std::string(_name), _p));
	}

	void GMetaclass::addEditableProperty(const char * _name, GMetaproperty * _p)
	{
		assert(_name && "GMetaclass::addEditableProperty name must not be null");
		assert(_p && "GMetaclass::addEditableProperty GMetaproperty must not be null");

		Gmetaeditableproperties[properties_version].insert(std::make_pair(std::string(_name), _p));
	}

	GMetaclass::GMetaclass():name("")
	{
		properties_version = 0;
	}
}

#include <GFramework/GReflection/GReflection.h>


namespace GFramework
{
	std::map<std::string, GMetaclass*> Gmetatable;

	template<>
	void GMetafunction_Base::iterate_arg_type<void>(lua_State* L, std::vector<GVariant>& args, int offset) {

	}

#if 0
	GMetaclassList::GMetaclassList()
	{

	}

	void GMetaclassList::print(std::string indent)
	{
		for (auto c : Gmetatable)
		{
			std::cout << indent << "Class: " << c.first << std::endl;
		}
	}

	GMetaclassList::~GMetaclassList()
	{

	}
#endif
	GReflection::GReflection()
	{
	}

	GReflection::~GReflection()
	{
	}

	void GMetaclass::getConstructorsList(std::vector<std::string> &constructors_list)
	{
		for (auto it = Gmetaconstructors.begin(); it != Gmetaconstructors.end(); ++it) {
			constructors_list.push_back(it->first);
		}
		return;
	}

	void GMetaclass::getFunctionsList(std::vector<std::string> &functions_list)
	{
		for (auto baseclass : GBaseMetaclasses)
		{
			//GMetaclass* basemetaclass = GMetaclassList::instance().getMetaclass(baseclass.end()->c_str());
			GMetaclass* basemetaclass = GMetaclass::getBaseMetaclass(baseclass);
			if (basemetaclass)
				basemetaclass->getFunctionsList(functions_list);
		}
		for (auto it = publicMemberFunctions.begin(); it != publicMemberFunctions.end(); ++it) {
			functions_list.push_back(it->first);
		}
		return;
	}

	void GMetaclass::getStaticFunctionsList(std::vector<std::string> &functions_list)
	{
		for (auto baseclass : GBaseMetaclasses)
		{
			GMetaclass* basemetaclass = GMetaclass::getBaseMetaclass(baseclass);
			if (basemetaclass)
				basemetaclass->getStaticFunctionsList(functions_list);
		}
		for (auto it = staticMemberFunctions.begin(); it != staticMemberFunctions.end(); ++it) {
			functions_list.push_back(it->first);
		}
		return;
	}

	void GMetaclass::getPropertiesList(std::vector<std::string> &properties_list)
	{
		for (auto baseclass : GBaseMetaclasses)
		{
			GMetaclass* basemetaclass = GMetaclass::getBaseMetaclass(baseclass);
			if (basemetaclass)
				basemetaclass->getPropertiesList(properties_list);
		}
		for (auto it1 = Gmetaproperties.begin(); it1 != Gmetaproperties.end(); ++it1) {
			for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
				properties_list.push_back(it2->first);
			}
		}
		return;
	}

	void GMetaclass::getEditablePropertiesList(std::vector<std::string> &properties_list)
	{
		for (auto baseclass : GBaseMetaclasses)
		{
			GMetaclass* basemetaclass = GMetaclass::getBaseMetaclass(baseclass);
			if (basemetaclass)
				basemetaclass->getEditablePropertiesList(properties_list);
		}
		for (auto it1 = Gmetaeditableproperties.begin(); it1 != Gmetaeditableproperties.end(); ++it1) {
			for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
				properties_list.push_back(it2->first);
			}
		}
		return;
	}

	unsigned int GMetaclass::getVersion()
	{
		return properties_version;
	}

	GMetaclass * GMetaclass::getBaseMetaclass(std::vector<std::string>& class_list)
	{
		GMetaclass *mc = nullptr;
		if (class_list.size() > 0)
		{
			GMetaNamespace nm = GMetaNamespaceList::_global()._namespace(class_list[0].c_str());
			size_t i = 1;
			for (; i < class_list.size() - 1; i++)
			{
				nm = GMetaNamespaceList::_global()._namespace(class_list[i].c_str());
			}
			mc = nm.getMetaclass(class_list[i].c_str());
		}
		else
		{
			GMetaNamespace nm = GMetaNamespaceList::_global();
			mc = nm.getMetaclass(class_list[0].c_str());
		}

		return mc;
	}

	std::string GMetaclass::getFullNamespace()
	{
		std::string nmstr = name;
		GMetaNamespace* nm = parentNamespace;
		while (nm != nullptr)
		{
			if(std::string(nm->getName()) != "")
			{
				nmstr.insert(0, ":");
				nmstr.insert(0, nm->getName());
			}
			nm = nm->getParentNamespace();
		}
		return nmstr;
	}

	void GMetaclass::addConstructor(const char* _name, GMetaconstructor* _c)
	{
		assert(_name && "GMetaclass::addConstructor name must not be null");
		assert(_c && "GMetaclass::addConstructor GMetaconstructor must not be null");

		Gmetaconstructors.insert(std::make_pair(std::string(_name), _c));
	}

	void GMetaclass::addPublicFunction(const char * _name, GMetaMemberfunction * _f)
	{
		assert(_name && "GMetaclass::addPublicFunction name must not be null");
		assert(_f && "GMetaclass::addPublicFunction GMetaMemberfunction must not be null");

		publicMemberFunctions.insert(std::make_pair(std::string(_name), _f));
	}

	void GMetaclass::addProtectedFunction(const char * _name, GMetaMemberfunction * _f)
	{
		assert(_name && "GMetaclass::addProtectedFunction name must not be null");
		assert(_f && "GMetaclass::addProtectedFunction GMetaMemberfunction must not be null");

		protectedMemberFunctions.insert(std::make_pair(std::string(_name), _f));
	}

	void GMetaclass::addStaticFunction(const char * _name, GMetaStaticfunction * _f)
	{
		assert(_name && "GMetaclass::addStaticFunction name must not be null");
		assert(_f && "GMetaclass::addStaticFunction GMetaStaticfunction must not be null");

		staticMemberFunctions.insert(std::make_pair(std::string(_name), _f));
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
		parentNamespace = nullptr;
	}

	GMetaNamespaceList::GMetaNamespaceList() {

	}

	GMetaNamespace& GMetaNamespaceList::_global()
	{
		static GMetaNamespace globalNamespace("");
		return globalNamespace;
	}

	void GMetaNamespaceList::print(std::string indent)
	{
		std::string this_indent = indent;
		this_indent.append("\t");
		for (auto p : namespaceList)
		{
			std::cout << indent << "Namespace: " << p.first << std::endl;
			p.second->print(this_indent);
		}
	}

	GMetaNamespace& GMetaNamespaceList::_namespace(const char* namespace_name)
	{
		auto itr = namespaceList.find(namespace_name);
		if (itr != namespaceList.end())
			return *(itr->second);
		return *(namespaceList.insert(std::pair<std::string, GMetaNamespace*>(namespace_name, new GMetaNamespace(namespace_name))).first->second);
	}

	GMetaNamespace::GMetaNamespace(const char* namespace_name) {
		assert(namespace_name);
		name = namespace_name;
		parentNamespace = nullptr;
		//metaclassList = new GMetaclassList();
	}

	GMetaNamespace::~GMetaNamespace()
	{
		//delete metaclassList;
	}

	const char* GMetaNamespace::getName() const {
		return name.c_str();
	}

	/*GMetaclassList& GMetaNamespace::getMetaclassList() {
		return *metaclassList;
	}*/

	GMetaNamespace& GMetaNamespace::_namespace(const char* namespace_name)
	{
		GMetaNamespace* childNamespace = &(namespaceList._namespace(namespace_name));
		childNamespace->parentNamespace = this;
		childMetanamespacelist[std::string(namespace_name) ] = childNamespace;
		return *childNamespace;
	}

	void GMetaNamespace::print(std::string indent)
	{
		namespaceList.print(indent);
		for (auto f : metafunctionsList)
		{
			std::cout << indent << "Function: " << f.first << std::endl;
		}

		for (auto f : metaclasslist)
		{
			std::cout << indent << "Class: " << f.first << std::endl;
		}
	}

	GMetafunction * GMetaNamespace::getMetaFunction(const char * function_name)
	{
		assert(function_name);
		auto itr = metafunctionsList.find(function_name);
		if (itr == metafunctionsList.end()) 
		{
			return nullptr;
		}
		return itr->second;
	}
	GMetaNamespace * GMetaNamespace::getMetaNamespace(const char * namespace_name)
	{
		GMetaNamespace * nm = nullptr;
		auto itr = childMetanamespacelist.find(std::string(namespace_name));
		if (itr != childMetanamespacelist.end())
		{
			nm = itr->second;
		}
		return nm;
	}
	GMetaNamespace * GMetaNamespace::getParentNamespace()
	{
		return parentNamespace;
	}
}

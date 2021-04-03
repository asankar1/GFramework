#include <iostream>
#include <GFramework/GReflection/GReflection.h>
#include <GFramework/GScript/GLuaScript.h>
//#include <sphere.h>
using namespace std;

namespace GFramework
{
#if 1
	int member_function(lua_State *L);
	int static_member_function(lua_State *L);
	static vector<string> lua_tables_name_list;

	GLuaState::GLuaState()
	{
		L = luaL_newstate();
		luaL_openlibs(L);
	}

	void GLuaState::add_namespace(const char* name)
	{
		assert(name && "name for GLuaState::add_namespace cannot be null");

		lua_getglobal(L, name);
		luaL_newmetatable(L, name);
		lua_pushstring(L, "__index");
		lua_newtable(L);
		lua_settable(L, -3);
		lua_setmetatable(L, -2);
	}

	void GLuaState::create_metatable(const char* name)
	{
		lua_getglobal(L, name);
		luaL_newmetatable(L, name);
		lua_pushstring(L, "__index");
		lua_newtable(L);
		lua_settable(L, -3);
		lua_setmetatable(L, -2);
		//lua_settop(L, 0);
	}

	void GLuaState::add_integer_to_metatable(const char* mt_name, const char* int_name, int value)
	{
		lua_getglobal(L, mt_name);
		if (LUA_TNIL == luaL_getmetafield(L, -1, "__index"))
			return;

		lua_pushstring(L, int_name);
		lua_pushinteger(L, value);
		lua_settable(L, -3);
	}

	void GLuaState::add_metatable_to_metatable(const char* mt_name, const char* tb_name, const char* value)
	{
		lua_getglobal(L, mt_name);
		if (LUA_TNIL == luaL_getmetafield(L, -1, "__index"))
			return;
		lua_pushstring(L, tb_name);
		luaL_getmetatable(L, value); 
		lua_settable(L, -3);
	}

	void GLuaState::add_constructors_metatable()
	{
		const int entry_top = lua_gettop(L);
		int top = 0;
		lua_getglobal(L, "new"); top = lua_gettop(L);
		luaL_newmetatable(L, "new"); top = lua_gettop(L);
		{
			lua_pushstring(L, "__index"); top = lua_gettop(L);
			lua_newtable(L); top = lua_gettop(L);
#if 1
			{
				lua_pushstring(L, "count"); top = lua_gettop(L);
				lua_pushinteger(L, 1); top = lua_gettop(L);
				lua_settable(L, -3); top = lua_gettop(L);
			}
#endif
#if 1
			{
				lua_pushstring(L, "types"); top = lua_gettop(L);
				luaL_newmetatable(L, "types"); top = lua_gettop(L);
				{
					lua_pushstring(L, "__index"); top = lua_gettop(L);
					lua_newtable(L); top = lua_gettop(L);
					{
						lua_pushstring(L, "count"); top = lua_gettop(L);
						lua_pushinteger(L, 2); top = lua_gettop(L);
						lua_settable(L, -3); top = lua_gettop(L);
					}
				}
				lua_settable(L, -3);
				//lua_getglobal(L, "types"); top = lua_gettop(L);
				luaL_getmetatable(L, "types"); top = lua_gettop(L);
				lua_setmetatable(L, -2); top = lua_gettop(L);
				lua_settable(L, -3); top = lua_gettop(L);

			}
#endif
			lua_settable(L, -3); top = lua_gettop(L);
		}
		lua_setmetatable(L, -2); top = lua_gettop(L);

	}

	void GLuaState::register_script_for_metaclass(GMetaclass* metaclass)
	{
		luaL_newmetatable(L, metaclass->getName().c_str());

		{
			lua_pushstring(L, "__index");
			lua_newtable(L);

			std::vector<std::string> functions_list;
			metaclass->getFunctionsList(functions_list);
			for (auto it = functions_list.begin(); it != functions_list.end(); ++it)
			{
				auto m = metaclass->getPublicMemberFunction(it->c_str());
				lua_pushstring(L, it->c_str());
				lua_pushlightuserdata(L, m);
				lua_pushcclosure(L, member_function, 1);
				lua_settable(L, -3);
			}

			std::vector<std::string> static_functions_list;
			metaclass->getStaticFunctionsList(static_functions_list);

			for (auto it = static_functions_list.begin(); it != static_functions_list.end(); ++it)
			{
				auto m = metaclass->getStaticFunction(it->c_str());
				lua_pushstring(L, it->c_str());
				lua_pushlightuserdata(L, m);
				lua_pushcclosure(L, static_member_function, 1);
				lua_settable(L, -3);
			}

			lua_settable(L, -3);
		}

		string ss = metaclass->getName();
		ss = ss.append("_metatable");
		luaL_newmetatable(L, ss.c_str());
		{
			lua_pushstring(L, "__index");
			lua_newtable(L);

			std::vector<std::string> static_functions_list;
			metaclass->getStaticFunctionsList(static_functions_list);
			for (auto it = static_functions_list.begin(); it != static_functions_list.end(); ++it)
			{
				auto m = metaclass->getStaticFunction(it->c_str());
				lua_pushstring(L, it->c_str());
				lua_pushlightuserdata(L, m);
				lua_pushcclosure(L, static_member_function, 1);
				lua_settable(L, -3);
			}
			lua_settable(L, -3);
		}
		string nn = metaclass->getName();
		nn.append("_class");
		lua_newuserdata(L, nn.length());
		lua_setglobal(L, metaclass->getName().c_str());


		lua_getglobal(L, metaclass->getName().c_str());
		luaL_getmetatable(L, ss.c_str());
		lua_setmetatable(L, -2);

#if 0
		string ss = metaclass->getName();
		ss = ss.append("_metatable");
		luaL_newmetatable(L, ss.c_str());
		lua_pushstring(L, "__index");
		lua_newtable(L);
		for (auto it = static_functions_list.begin(); it != static_functions_list.end(); ++it)
		{
			GMetafunction* m = metaclass->getStaticFunction(it->c_str());
			lua_pushstring(L, it->c_str());
			lua_pushlightuserdata(L, m);
			lua_pushcclosure(L, static_member_function, 1);
			lua_settable(L, -3);
		}
		lua_settable(L, -3);

		lua_pushstring(L, metaclass->getName().c_str());
		luaL_getmetatable(L, ss.c_str());
		lua_setmetatable(L, -2);

		luaL_getmetatable(L, ss.c_str());
		lua_setglobal(L, metaclass->getName().c_str());
#endif
	}

	GLuaState::~GLuaState()
	{
		lua_close(L);
	}
#endif
	template<>
	void Glua_pusher<void>(lua_State* L, GVariant& rv)
	{
		lua_pushnil(L);
	}

	template<>
	int Glua_puller<unsigned int>(lua_State* L, std::vector<GVariant>& args, int offset)
	{
		int top = lua_gettop(L);
		unsigned int next_argument_index = args.size() + offset;
		if (!lua_isnumber(L, next_argument_index))
			return 0;
		unsigned int value = lua_tonumber(L, next_argument_index);
		args.push_back(GVariant(value));
		return 1;
	}

	template<>
	int Glua_puller<int>(lua_State* L, std::vector<GVariant>& args, int offset)
	{
		int top = lua_gettop(L);
		unsigned int next_argument_index = args.size() + offset;
		if (!lua_isnumber(L, next_argument_index))
			return 0;
		int value = lua_tonumber(L, next_argument_index);
		args.push_back(GVariant(value));
		return 1;
	}

	template<>
	int Glua_puller<float>(lua_State* L, std::vector<GVariant>& args, int offset)
	{
		int top = lua_gettop(L);
		unsigned int next_argument_index = args.size() + offset;
		if (!lua_isnumber(L, next_argument_index))
			return 0;
		float value = lua_tonumber(L, next_argument_index);
		args.push_back(GVariant(value));
		return 1;
	}

	template<>
	int Glua_puller<std::string>(lua_State* L, std::vector<GVariant>& args, int offset)
	{
		int top = lua_gettop(L);
		unsigned int next_argument_index = args.size() + offset;
		if (!lua_isstring(L, next_argument_index))
			return 0;
		std::string value(lua_tostring(L, next_argument_index));
		args.push_back(GVariant(value));
		return 1;
	}

	int testf(lua_State *L)
	{
		cout << "####### testf #######" << endl;
		//int top = lua_gettop(L);
		int k = 0;
		for (int i = 0; i < k; i++)
		{
			const char* gg = lua_tostring(L, -(i + 1));
		}
		return 1;
	}

#if 0
	int add_metaclass_static_functions(lua_State *L)
	{

		/*vector<luaL_Reg> GPropertiesList;
		std::vector<std::string> static_functions_list;
		metaclass->getStaticFunctionsList(static_functions_list);
		for (auto it = static_functions_list.begin(); it != static_functions_list.end(); ++it)
		{
			GMetafunction* m = metaclass->getStaticFunction(it->c_str());
			GPropertiesList.push_back({ m->getName().c_str(), testf });
		}
		GPropertiesList.push_back({ NULL, NULL });
		luaL_newlib(L, GPropertiesList.data());*/


		return 1;
	}


#endif
	int member_function(lua_State *L)
	{
		GMetafunction* m = (GMetafunction*)lua_touserdata(L, lua_upvalueindex(1));
		int top = lua_gettop(L);
		if (!lua_isuserdata(L, 1))
		{
			return 0;
		}

		m->invoke_lua(L);

		return 1;
	}

	int static_member_function(lua_State *L)
	{
		auto m = (GMetafunction*)lua_touserdata(L, lua_upvalueindex(1));

		m->invoke_lua(L);

		return 1;
	}

#if 0
	static int new_class(lua_State *L) {
		luaL_newmetatable(L, "class_type");
		
		const char * name = luaL_checkstring(L, -2);

		GMetaclass *classmeta = GMetaclassList::instance().getMetaclass(name);

		luaL_getmetatable(L, "class_type");
		lua_setmetatable(L, -2);


		//1 / 0;
		return 1;  /* new userdatum is already on the stack */
	}
#endif

	static int register_lua_class_module(lua_State *L)
	{
		vector<luaL_Reg> GPropertiesList;

		//GPropertiesList.push_back({ "new_class", new_class });

		GPropertiesList.push_back({ NULL, NULL });

		luaL_newlib(L, GPropertiesList.data());

		return 1;
	}

#if 0
	void GLuaState::open_lua_classes_module()
	{
		lock_guard<std::mutex> lock(instance->GLuaMutex);

		luaL_requiref(instance->L, "Classes", register_lua_class_module, 1);
	}
#endif

	GLuaScriptChunk::GLuaScriptChunk(const std::string & script) : chunk(script)
	{
		L = luaL_newstate();
		if (luaL_loadstring(L, chunk.c_str()) || lua_pcall(L, 0, 0, 0)) {
			std::cout << "Error: script not loaded (" << script << ")" << std::endl;
			L = 0;
		}
	}

	GLuaScriptChunk::~GLuaScriptChunk()
	{
		if (L) lua_close(L);
	}

	void GLuaScriptChunk::printError(const std::string& variableName, const std::string& reason) {
		std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
	}

	GLuaScript::GLuaScript()
	{
		L = luaL_newstate();
		luaL_openlibs(L);
	}

	GLuaScript::~GLuaScript()
	{
		lua_close(L);
	}

	void GLuaScript::printError(const std::string & variableName, const std::string & reason)
	{
		std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
	}
	GLuaScriptFile::GLuaScriptFile(const char * _file)
	{
		file = _file;
	}

	GLuaScriptFile::~GLuaScriptFile()
	{

	}

	bool GLuaScriptFile::load()
	{
		lock_guard<std::mutex> lock(luaStateMutex);

		int error = luaL_loadfile(L, file);
		if (error) // if non-0, then an error
		{
			// the top of the stack should be the error string
			if (lua_isstring(L, lua_gettop(L)))
			{
				// get the top of the stack as the error and pop it off
				string str = lua_tostring(L, lua_gettop(L));
				lua_pop(L, 1);
				cout << str;
			}
			return false;
		}
		return true;
	}

	bool GLuaScriptFile::run()
	{
		lock_guard<std::mutex> lock(luaStateMutex);
		int result = lua_pcall(L, 0, LUA_MULTRET, 0); // once again, returns non-0 on error, you should probably add a little check
		switch (result)
		{
		case LUA_ERRRUN:
			cout << "Below runtime error occured!\n";
			cout << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);
			return false;
			break;
		}
		return true;
	}

	GLuaModule::GLuaModule(const char * _name)
	{
		name = _name;
	}

	GLuaModule::~GLuaModule()
	{
	
	}

	void GLuaModule::add_namespace(const char * _name)
	{
		namespaceList.insert(_name);
	}

	const std::vector<luaL_Reg>& GLuaModule::getTokensList() const
	{
		return tokensList;
	}

	void GLuaModule::exportToLua(lua_State *L)
	{
		//luaL_requiref(L, name, register_lua_test_module, 1);
	}

	int GLuaModule::pushNamespaceToLua(lua_State * L)
	{
		return 0;
	}
}

#include <iostream>
#include <Greflection.h>
#include "GLuaScript.h"
#include <sphere.h>
using namespace std;

namespace GFramework
{
	lua_State* GLuaState::L = nullptr;
	int member_function(lua_State *L);
	int static_member_function(lua_State *L);
	static vector<string> lua_tables_name_list;

	GLuaState::~GLuaState()
	{
		lua_close(L);
	}

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

	lua_State* GLuaState::getState()
	{
		if (L == nullptr)
		{
			L = luaL_newstate();
		}

		return L;
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

	void register_script_for_metaclass(GMetaclass* metaclass)
	{
		lua_State* L = GLuaState::getState();
		luaL_newmetatable(L, metaclass->getName().c_str());

		{
			lua_pushstring(L, "__index");
			lua_newtable(L);

			std::vector<std::string> functions_list;
			metaclass->getFunctionsList(functions_list);
			for (auto it = functions_list.begin(); it != functions_list.end(); ++it)
			{
				auto m = metaclass->getMemberFunction(it->c_str());
				lua_pushstring(L, it->c_str());
				lua_pushlightuserdata(L, m);
				lua_pushcclosure(L, member_function, 1);
				lua_settable(L, -3);
			}

			std::vector<std::string> static_functions_list;
			metaclass->getStaticFunctionsList(static_functions_list);
			if (static_functions_list.empty())
			{
				return;
			}
#if 1
			for (auto it = static_functions_list.begin(); it != static_functions_list.end(); ++it)
			{
				auto m = metaclass->getStaticFunction(it->c_str());
				lua_pushstring(L, it->c_str());
				lua_pushlightuserdata(L, m);
				lua_pushcclosure(L, static_member_function, 1);
				lua_settable(L, -3);
			}
#else
			luaL_requiref(L, metaclass->getName().c_str(), [](lua_State *L) {
				const char* name = lua_tostring(L, -1);
				GMetaclass* metaclass = GMetaclassList::instance().getMetaclass(name);
				std::vector<std::string> static_functions_list;
				metaclass->getStaticFunctionsList(static_functions_list);
				if (static_functions_list.empty())
				{
					return 1;
				}
				std::vector<luaL_Reg> GPropertiesList;
				for (auto it = static_functions_list.begin(); it != static_functions_list.end(); ++it)
				{
					GMetafunction* m = metaclass->getStaticFunction(it->c_str());
					lua_pushstring(L, it->c_str());
					lua_pushlightuserdata(L, m);
					lua_pushcclosure(L, static_member_function, 1);
					lua_settable(L, -3);
				}


				GPropertiesList.push_back({ NULL, NULL });
				luaL_newlib(L, GPropertiesList.data());

				return 1;
			}

			, 1);
#endif
			lua_settable(L, -3);
		}

		string ss = metaclass->getName();
		ss = ss.append("_metatable");
		luaL_newmetatable(L, ss.c_str());
		lua_pushstring(L, "__index");
		lua_newtable(L);

		std::vector<std::string> static_functions_list;
		metaclass->getStaticFunctionsList(static_functions_list);
		if (static_functions_list.empty())
		{
			return;
		}
		for (auto it = static_functions_list.begin(); it != static_functions_list.end(); ++it)
		{
			auto m = metaclass->getStaticFunction(it->c_str());
			lua_pushstring(L, it->c_str());
			lua_pushlightuserdata(L, m);
			lua_pushcclosure(L, static_member_function, 1);
			lua_settable(L, -3);
		}
		lua_settable(L, -3);

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

	static int new_class(lua_State *L) {
		luaL_newmetatable(L, "class_type");
		
		const char * name = luaL_checkstring(L, -2);

		GMetaclass *classmeta = GMetaclassList::instance().getMetaclass(name);

		luaL_getmetatable(L, "class_type");
		lua_setmetatable(L, -2);


		//1 / 0;
		return 1;  /* new userdatum is already on the stack */
	}

	static int register_lua_class_module(lua_State *L)
	{
		vector<luaL_Reg> GPropertiesList;

		GPropertiesList.push_back({ "new_class", new_class });

		GPropertiesList.push_back({ NULL, NULL });

		luaL_newlib(L, GPropertiesList.data());

		return 1;
	}

	void open_lua_classes_module(lua_State* L)
	{
		luaL_requiref(L, "Classes", register_lua_class_module, 1);
	}

	GLuaScript::GLuaScript(const std::string & script) : chunk(script)
	{
		L = luaL_newstate();
		if (luaL_loadstring(L, chunk.c_str()) || lua_pcall(L, 0, 0, 0)) {
			std::cout << "Error: script not loaded (" << script << ")" << std::endl;
			L = 0;
		}
	}

	GLuaScript::~GLuaScript()
	{
		if (L) lua_close(L);
	}

	void GLuaScript::printError(const std::string& variableName, const std::string& reason) {
		std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
	}
}

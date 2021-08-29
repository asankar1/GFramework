#pragma once
#include <assert.h>
#include <iostream>
#include <string>
#include <mutex>
#include <GFramework/GScript/GFrameworkLua.h>

namespace GFramework
{
	class GMetafunction;

	class GFRAMEWORK_API GLuaInterface
	{

	public:
		static GLuaInterface& getInstance() {
			static GLuaInterface instance;
			return instance;
		}

		~GLuaInterface()
		{
			const std::lock_guard<std::mutex> lock(GLuaStateMutex);
			lua_close(L);
		}

		bool loadScriptFile(const char* filepath)
		{
			int error = luaL_loadfile(L, filepath);
			if (error) // if non-0, then an error
			{
				// the top of the stack should be the error string
				if (lua_isstring(L, lua_gettop(L)))
				{
					// get the top of the stack as the error and pop it off
					std::string err = lua_tostring(L, lua_gettop(L));
					lua_pop(L, 1);
					std::cout << "Below error occured while loading the lua script: '" << filepath << "'\n";
					std::cout << err;
				}
				else
				{
					std::cout << "Some unknown error occured while loading the lua script: '" << filepath << "'\n";
				}
				return false;
			}
			else
			{
				// if not an error, then the top of the stack will be the function to call to run the file
				int result = lua_pcall(L, 0, LUA_MULTRET, 0); // once again, returns non-0 on error, you should probably add a little check
				if (result != LUA_OK)
				{
					switch (result)
					{
					case LUA_ERRRUN:
						std::cout << "Below runtime error occured while running the lua script: '" << filepath << "'\n";
						std::cout << lua_tostring(L, -1) << std::endl;
						lua_pop(L, 1);
						break;
					case LUA_ERRMEM:
						std::cout << "Memory error occured while running the lua script: '" << filepath << "'\n";
						break;
					case LUA_ERRERR:
						std::cout << "Some error message handler error occured while running the lua script: '" << filepath << "'\n";
						break;
					}
					return false;
				}
			}
			return true;
		}

		void addNamespace(const char* name)
		{
			const std::lock_guard<std::mutex> lock(GLuaStateMutex);
			assert(name && "name argument for GLuaScript::addNamespace cannot be null");
			auto v = lua_getglobal(L, name);
			if (v == LUA_TNIL)
			{
				lua_newtable(L);
				lua_setglobal(L, name);
				lua_getglobal(L, name);
			}
			else if(v != LUA_TTABLE)
			{
				assert(name && "name is not a namespace table");
			}


			/*luaL_newmetatable(L, name);
			lua_pushstring(L, "__index");
			lua_newtable(L);
			lua_settable(L, -3);
			lua_setmetatable(L, -2);*/
		}

		void addFunction(const char* name, GMetafunction* f);

		template<typename FUNC>
		static int functionCaller(FUNC f)
		{
			f();
			return 0;
		}

	public:
		std::mutex GLuaStateMutex;

	private:
		GLuaInterface()
		{
			L = luaL_newstate();
			luaL_openlibs(L);
		}

	private:
		lua_State* L;
	};

	class GFRAMEWORK_API GLuaScript
	{
	
	public:
		GLuaScript& getInstance() {
			static GLuaScript instance;
			return instance;
		}

		~GLuaScript()
		{
			const std::lock_guard<std::mutex> lock(GLuaStateMutex);
			lua_close(L);
		}

		void addNamespace(const char* name)
		{
			const std::lock_guard<std::mutex> lock(GLuaStateMutex);
			assert(name && "name argument for GLuaScript::addNamespace cannot be null");

			lua_getglobal(L, name);
			if (lua_isnil(L, -1))
			{
				lua_pop(L, 1);
				lua_newtable(L);
			}
			luaL_newmetatable(L, name);
			lua_pushstring(L, "__index");
			lua_newtable(L);
			lua_settable(L, -3);
			lua_setmetatable(L, -2);
		}

	public:
		std::mutex GLuaStateMutex;

	private:
		GLuaScript()
		{
			L = luaL_newstate();
			luaL_openlibs(L);
		}

	private:
		lua_State* L;
	};

	class GFRAMEWORK_API GScriptLibrary
	{
	public:
		GScriptLibrary(const char* library_name) : name(library_name) {

		}
	protected:
		const std::string name;
	};
}
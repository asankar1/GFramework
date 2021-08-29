#pragma once
#include <assert.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <GFramework/GScript/GFrameworkLua.h>

namespace GFramework
{
	class GMetafunction;

	class GFRAMEWORK_API GScriptEngine
	{
	public:
		GScriptEngine(const char* _name) : name(_name){

		}

		const char* getName() const {
			return name;
		}

		virtual bool addNamespace(const char* name) = 0;

		virtual bool addFunction(const char* name, GMetafunction* f) = 0;
	
		virtual bool runScriptFile(const char* filepath) = 0;

	private:
		const char* name;
	};

	class GFRAMEWORK_API GLuaEngine : public GScriptEngine
	{
	public:
		~GLuaEngine()
		{
			const std::lock_guard<std::mutex> lock(GLuaStateMutex);
			lua_close(L);
		}

		static GLuaEngine& getInstance() {
			static GLuaEngine instance("Lua");
			return instance;
		}

		virtual bool addNamespace(const char* name) override
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
			else if (v != LUA_TTABLE)
			{
				assert(name && "name is not a namespace table");
			}

			return true;
		}

		virtual bool addFunction(const char* name, GMetafunction* f) override;

		virtual bool runScriptFile(const char* filepath) override
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

	private:
		GLuaEngine(const char* _name) : GScriptEngine(_name) 
		{
			L = luaL_newstate();
			luaL_openlibs(L);
			//GScriptInterface::getInstance().addScriptEngine(".lua", this);
		}

	public:
		std::mutex GLuaStateMutex;

	private:
		lua_State* L;
	};

	class GFRAMEWORK_API GScriptInterface
	{
	public:
		static GScriptInterface& getInstance() {
			static GScriptInterface instance;
			return instance;
		}

		bool runScriptFile(const char* filepath)
		{
			auto ext = getScriptExtension(filepath);
			if (ext.empty())
			{
				std::cout << "Unable to get the extension of the script '" << filepath << "'" << std::endl;
				return false;
			}

			GScriptEngine* engine = nullptr;
			for (auto _pair : engineExtensionMap)
			{
				if (_pair.second == ext)
				{
					engine = _pair.first;
					break;
				}
			}

			if (!engine)
			{
				std::cout << "Unable to find a suitable script engine for the script '" << filepath << "'" << std::endl;
				return false;
			}

			return engine->runScriptFile(filepath);
		}

		bool addScriptEngine(std::string ext, GScriptEngine* engine)
		{
			if (engine == nullptr)
			{
				std::cout << "The script engine passed for the extension '" << ext << "' is null" << std::endl;
				return false;
			}

			auto itr = engineExtensionMap.find(engine);
			if (itr != engineExtensionMap.end())
			{
				std::cout << "A script engine '" << engine->getName() << "' is already assigned for the extension '" << ext << "'" << std::endl;
				return false;
			}

			engineExtensionMap[engine] = ext;

			return true;
		}

		~GScriptInterface() {
		}

		bool addNamespace(const char* name)
		{
			bool result = true;
			for (auto _pair : engineExtensionMap)
			{
				if (!_pair.first->addNamespace(name))
				{
					std::cout << "addNamespace failed for script engine '" << _pair.first->getName() << "' for the argument '" << name << "'" << std::endl;
					result = false;
				}
			}

			return result;
		}

		bool addFunction(const char* name, GMetafunction* f)
		{
			bool result = true;
			for (auto _pair : engineExtensionMap)
			{
				if (!_pair.first->addFunction(name, f))
				{
					std::cout << "addNamespace failed for script engine '" << _pair.first->getName() << "' for the argument '" << name << "'" << std::endl;
					result = false;
				}
			}

			return result;
		}

	private:
		GScriptInterface() {
			addScriptEngine(".lua", &GLuaEngine::getInstance());
		}

		std::string getScriptExtension(std::string filepath)
		{
			auto pos = filepath.rfind(".");
			if (pos != std::string::npos)
			{
				auto ext = filepath.substr(pos);
				return ext;
			}
			return std::string();
		}

		std::map<GScriptEngine*, std::string> engineExtensionMap;
	};

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
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GFrameworkLua.h>
#include <GLua_data_exchange.h>
#include <GVariant.h>

namespace GFramework
{
	class GMetaclass;

	class GLuaState
	{
	public:
		static lua_State* getState();
		~GLuaState();
	private:
		GLuaState() {} 
		static lua_State* L;
	};
	
	class GLuaScript
	{
	public:
		GLuaScript(const std::string& script);
		void printError(const std::string& variableName, const std::string& reason);
		~GLuaScript();
	protected:
		const std::string chunk;
		lua_State* L;
	};

	void open_lua_classes_module(lua_State* L);

	void register_script_for_metaclass(GMetaclass* metaclass);
}


#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GScript/GFrameworkLua.h>
#include <GScript/GLua_data_exchange.h>
#include <GVariant/GVariant.h>

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

	void GFRAMEWORK_API open_lua_classes_module(lua_State* L);

	void GFRAMEWORK_API register_script_for_metaclass(GMetaclass* metaclass);
}


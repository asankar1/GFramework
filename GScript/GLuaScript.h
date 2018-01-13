#pragma once
#include <iostream>
#include <string>
#include <GFrameworkLua.h>
using namespace std;

namespace GFramework
{
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

	int luaopen_vector2d(lua_State *L);
}


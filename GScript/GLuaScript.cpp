#include <iostream>
#include "GLuaScript.h"

namespace GFramework
{
	class vector2d
	{
	public:
		vector2d() {
			x = 0.0f; y = 0.0f;
		}
		void set(float _x, float _y) {
			x = _x; y = _y;
		}
		void print() {
			cout << "x:" << x << ", y:" << y << endl;
		}
		float x;
		float y;
	};

	static int index(lua_State *L)
	{
		size_t s= 0;
		vector2d* userdata = (vector2d*)luaL_checkudata(L, 1, "vector2d_type");
		const char* keyname = luaL_checklstring(L, 2, &s);
		return 0;
	}

	static int newvector2d(lua_State *L) {
		luaL_newmetatable(L, "vector2d_type");
		lua_pushstring(L, "__index");
		lua_pushcfunction(L, index);
		lua_settable(L, -3);  /* metatable.__index = metatable */

		size_t nbytes = sizeof(vector2d);
		vector2d* userdata = (vector2d*) lua_newuserdata(L, nbytes);
		userdata->x = 1;
		userdata->y = 2;
		luaL_getmetatable(L, "vector2d_type");
		lua_setmetatable(L, -2);


		//1 / 0;
		return 1;  /* new userdatum is already on the stack */
	}

	/*
	static int newvector2d(lua_State *L) {
	size_t nbytes = sizeof(vector2d);
	//luaL_newmetatable(L, "vector2d_type");

	lua_newtable(L);

	lua_pushstring(L, "x");
	lua_pushnumber(L,2);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, 3);
	lua_settable(L, -3);

	//1 / 0;
	return 1;  
}
	*/
	static int printvector2d(lua_State *L) {
		void *ud = luaL_checkudata(L, 1, "vector2d_type");
		luaL_argcheck(L, ud != NULL, 1, "`vector2d_type' expected");
		//vector2d *a = (vector2d *)lua_touserdata(L, -1);
		vector2d *a = (vector2d *)ud;
		a->print();
		return 1;  /* new userdatum is already on the stack */
	}

	int luaopen_vector2d(lua_State *L)
	{
		static const struct luaL_Reg vector2dlib[] = {
			{ "new", newvector2d },
			{ NULL, NULL }
		};

		luaL_newlib(L, vector2dlib);
#if 0
		//luaL_openlibs(L, "vector2d", vector2dlib, 0);

		lua_getglobal(L, "vector2d");
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
		}
		luaL_setfuncs(L, vector2dlib, 0);
		lua_setglobal(L, "vector2d");
#endif
		return 1;
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

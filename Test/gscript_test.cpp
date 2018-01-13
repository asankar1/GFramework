#include <string>
#include <iostream>
#include "gscript_test.h"
using namespace std;
using namespace GFramework;
/* Notes:
	> for every property type a meta table needs to be defined with meta methods:
		. __tostring for print
		. __metatable to prtect modifying metatable
*/
int add(lua_State *L);

int add(lua_State *L)
{
	double n1 = lua_tonumber(L, 1);
	double n2 = lua_tonumber(L, 2);

	lua_pushnumber(L, n1 + n2);

	return 1;
}

const char* script =	{ 
							"x=add(2,7)\n" 
							"print(x)\n"
						};

void tmain()
{
	lua_State* L = luaL_newstate();
	luaL_requiref(L, "vector2d", luaopen_vector2d, 1);
	luaL_openlibs(L);
	lua_pushcfunction(L, add);
	lua_setglobal(L, "add");

	//luaL_dostring(L, script);
/*	bool iswrong = luaL_dofile(L, "H:\\github\\GFramework\\GScript\\script.lua");
	if (iswrong)
	{
		cout << "Error in lua script\n";
	}*/


	int error = luaL_loadfile(L, "H:\\github\\GFramework\\GScript\\script.lua");
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
	}
	else
	{
		// if not an error, then the top of the stack will be the function to call to run the file
		int result = lua_pcall(L, 0, LUA_MULTRET, 0); // once again, returns non-0 on error, you should probably add a little check
		switch (result)
		{
		case LUA_ERRRUN:
			cout << "Below runtime error occured!\n";
			cout << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);
			break;
		}
	}

	lua_getglobal(L,"x");

	cout << lua_tonumber(L, -1) << endl;

	lua_close(L);
}

void run_script_testcases()
{
	tmain();

	cin.get();
}
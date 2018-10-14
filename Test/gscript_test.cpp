#include <string>
#include <iostream>
#include <GVariant.h>
#include <sphere.h>
#include "gscript_test.h"
using namespace std;
using namespace GFramework;
/* Notes:
	> for every property type a meta table needs to be defined with meta methods:
		. __tostring for print
		. __metatable to protect modifying metatable
*/

static int new_node(lua_State *L) {
	Node* s = new Node("node_srs", NodeSharedPtr(nullptr));
	size_t nbytes = sizeof(Glua_abstract_ptr);
	Glua_abstract_ptr* userdata = (Glua_abstract_ptr*)lua_newuserdata(L, nbytes);
	userdata->object = s;

	luaL_getmetatable(L, "node");
	lua_setmetatable(L, -2);

	return 1;
}

static int new_sphere(lua_State *L) {
	sphere* sp = new sphere("sphere_srs_parent", NodeSharedPtr(nullptr), 23);
	sphere* s = new sphere("sphere_srs", NodeSharedPtr(sp), 23);
	size_t nbytes = sizeof(Glua_abstract_ptr);
	Glua_abstract_ptr* userdata = (Glua_abstract_ptr*)lua_newuserdata(L, nbytes);
	userdata->object = s;

	luaL_getmetatable(L, "sphere");
	lua_setmetatable(L, -2);

	return 1;
}

int register_lua_test_module(lua_State *L)
{
	luaL_Reg GPropertiesList[] = {
		{ "new_node", new_node },
		{ "new_sphere", new_sphere },
		{ NULL, NULL }
	};

	luaL_newlib(L, GPropertiesList);

	return 1;
}

void open_lua_test_module(lua_State *L)
{
	luaL_requiref(L, "test", register_lua_test_module, 1);
}

void tmain()
{
	lua_State* L = GLuaState::getState();
	//luaL_requiref(L, "vector2d", luaopen_vector2d, 1);
	open_lua_variant_module(L);
	open_lua_classes_module(L);
	open_lua_test_module(L);
	luaL_openlibs(L);

	int error = luaL_loadfile(L, "../GScript/script.lua");
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
}

void run_script_testcases()
{
	tmain();

	cin.get();
}
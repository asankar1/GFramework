#include <string>
#include <iostream>

#include <gtest/gtest.h>
#include <GFramework/GVariant/GVariant.h>
#include <Sphere.h>
#include "gscript_test.h"
using namespace std;
using namespace GFramework;
using namespace GFrameworkTest;
/* Notes:
	> for every property type a meta table needs to be defined with meta methods:
		. __tostring for print
		. __metatable to protect modifying metatable
*/
#if 0
static int new_node(lua_State *L) {
	NodeSharedPtr n(nullptr);
	Node* s = new Node("node_srs", n);
	size_t nbytes = sizeof(Glua_abstract_ptr);
	Glua_abstract_ptr* userdata = (Glua_abstract_ptr*)lua_newuserdata(L, nbytes);
	userdata->object = s;

	luaL_getmetatable(L, "node");
	lua_setmetatable(L, -2);

	return 1;
}

static int new_sphere(lua_State *L) {
	NodeSharedPtr n(nullptr);
	sphere* sp = new sphere("sphere_srs_parent", n, 23);
	NodeSharedPtr n1(sp);
	sphere* s = new sphere("sphere_srs", n1, 23);
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

/*void open_lua_test_module(lua_State *L)
{
	luaL_requiref(L, "test", register_lua_test_module, 1);
}*/

int get_namespace(lua_State *L)
{
	auto T = lua_gettop(L);
	const char* namespace_name = luaL_checkstring(L, -1);
	T = lua_gettop(L);
	if (namespace_name == nullptr)
	{
		std::cout << "Error: getNamespace() function needs a string as argument" << std::endl;
		lua_pushnil(L);
		T = lua_gettop(L);
		return 1;
	}

	lua_getglobal(L, namespace_name); T = lua_gettop(L);
	luaL_newmetatable(L, namespace_name); T = lua_gettop(L);
	lua_pushstring(L, "__index"); T = lua_gettop(L);
	lua_newtable(L); T = lua_gettop(L);

	lua_pushstring(L, "value3");
	lua_pushinteger(L, 43);
	lua_settable(L, -3);

	lua_pushstring(L, "value1");
	lua_pushinteger(L, 1);
	lua_settable(L, -3);

	lua_pushstring(L, "value2");
	lua_pushinteger(L, 22);
	lua_settable(L, -3);

	lua_settable(L, -3); T = lua_gettop(L);
	lua_setmetatable(L, -2); T = lua_gettop(L);

	return 1;
}

int load_namespace(lua_State *L)
{
	const char* namespace_name = luaL_checkstring(L, -1);
	
	luaL_Reg functions_list[] = {
		{ "sample1", [](lua_State *L)->int { lua_pushstring(L, "SAMPLE1"); return 1; }},
		{ "sample2", [](lua_State *L)->int { lua_pushstring(L, "SAMPLE2"); return 1; }},
		{ "getNamespace", get_namespace},
		{ NULL, NULL }
	};

	luaL_newlib(L, functions_list);

	return 1;
}
#endif

class GScriptTest : public ::testing::Test {
protected:
	void SetUp() override {
	}

protected:
};

namespace ScriptTest
{
	void printcpp()
	{
		std::cout << "I am from CPP!\n";
	}
}

BEGIN_DEFINE_META_FUNCTION(ScriptTest)
	GMetaNamespaceList::_global()._namespace("ScriptTest")
		.function("printcpp", ScriptTest::printcpp)
	;
END_DEFINE_META_FUNCTION(ScriptTest)
/*struct n_metacreator
{ 
	GMetaNamespace * m; 
	n_metacreator() 
	{ 
		m = &GMetaNamespaceList::_global()._namespace("ScriptTest");
	} 
}; static const n_metacreator n_metacreator_;*/

TEST_F(GScriptTest, namespace_test)
{
	auto & script = GScriptInterface::getInstance();
	//lua.addNamespace("TestLib1");
	//lua.addFunction("printcpp", printcpp);
	EXPECT_EQ(true, script.runScriptFile("./script.lua"));
}

#if 1
#else
void run_script_testcases()
{
	GLuaState st;
	//st.add_constructors_metatable();

	//st.create_metatable("sphere");
	st.create_metatable("new");
	/*st.add_integer_to_metatable("new", "count3", 3);
	st.add_integer_to_metatable("new", "count8", 8);
	st.add_integer_to_metatable("new", "count1", 1);*/
	//st.add_metatable_to_metatable("new", "sphere", "sphere");

	lua_State* L = st.L;
	int error = luaL_loadfile(L, "../script.lua");
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
#if 0
	lua_State* L = luaL_newstate();
	auto T = lua_gettop(L);
	luaL_openlibs(L);
	T = lua_gettop(L);
	luaL_requiref(L, "test", load_namespace, 1);
	//lua_settop(L, 0);
	T = lua_gettop(L);
	int error = luaL_loadfile(L, "../../../GScript/script.lua");
	T = lua_gettop(L);
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
#endif
	//lua_State* L = GLuaState::getState();
	//luaL_requiref(L, "vector2d", luaopen_vector2d, 1);
	//open_lua_variant_module(L);
	//GLuaState::open_lua_classes_module();
	//open_lua_test_module(L);
	//luaL_openlibs(L);

	//GLuaState::add_namespace("GFramework");
#if 0
	add_constructors_metatable();
	create_metatable("new");
	add_integer_to_metatable("new", "count3", 3);

	int error = luaL_loadfile(L, "../../../GScript/script.lua");
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
#endif
}
#endif
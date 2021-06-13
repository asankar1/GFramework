#include <sstream>
#include <GFramework/GVariant/GVariant.h>
#include <GFramework/GVariant/GProperty.h>

using namespace std;

namespace GFramework
{
	static int index(lua_State *L)
	{
		size_t s = 0;
		const char* keyname = luaL_checklstring(L, 2, &s);
		return 1;
	}

	static int tostring(lua_State *L)
	{		
		lua_getmetatable(L, 1);
		lua_pushstring(L, "__name");
		lua_rawget(L, 2);
		const char * name = luaL_checkstring(L, -1);

		{
			GVec2Property* userdata = (GVec2Property*)luaL_testudata(L, 1, "Vec2_type");
			if (userdata)
			{
				std::ostringstream s;
				s << "x: " << userdata->getValue().x << " y: " << userdata->getValue().y;
				lua_pushstring(L, s.str().c_str());
				return 1;
			}
		}

		{
			GVec3Property* userdata = (GVec3Property*)luaL_testudata(L, 1, "Vec3_type");
			if (userdata)
			{
				std::ostringstream s;
				s << "x: " << userdata->getValue().x << " y: " << userdata->getValue().y;
				lua_pushstring(L, s.str().c_str());
				return 1;
			}
		}

		lua_pushstring(L, "!!Error! Attempt to print unknown GVariant type.!!");
		return 1;
	}

	static int newVec2(lua_State *L) {
		luaL_newmetatable(L, "Vec2_type");
		lua_pushstring(L, "__index");
		lua_pushcfunction(L, index);
		lua_settable(L, -3);  /* metatable.__index = metatable */

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, tostring);
		lua_settable(L, -3);  /* metatable.__index = metatable */
		

		size_t nbytes = sizeof(GVec2Property);
		GVec2Property* userdata = (GVec2Property*)lua_newuserdata(L, nbytes);
		userdata->setValue(glm::vec2(1, 11));

		luaL_getmetatable(L, "Vec2_type");
		lua_setmetatable(L, -2);

		return 1;
	}

	int register_lua_variant_module(lua_State *L)
	{
		vector<luaL_Reg> GPropertiesList;
		
		register_lua_script_functions(L, GPropertiesList);

		GPropertiesList.push_back({ NULL, NULL });

		luaL_newlib(L, GPropertiesList.data());

		return 1;
	}

	/*int open_lua_variant_module(lua_State *L)
	{
		luaL_requiref(L, "GVariant", register_lua_variant_module, 1);

		return 1;
	}*/
}
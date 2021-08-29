#include <GFramework/GReflection/GReflection.h>
#include <GFramework/GScript/GScript.h>

namespace GFramework
{
	bool GLuaEngine::addFunction(const char* name, GMetafunction* f)
	{
		lua_pushstring(L, name);
		lua_pushlightuserdata(L, f);
		lua_pushcclosure(L, [](lua_State* L)->int {
			auto m = (GMetafunction*)lua_touserdata(L, lua_upvalueindex(1));
			m->invoke_lua(L); return 0; }
		, 1);

		//lua_pushcfunction(L, [](lua_State* L)->int { std::cout << "I am from CPP lambda!\n"; return 0; });
		lua_settable(L, -3);
		return true;
	}

	void GLuaInterface::addFunction(const char* name, GMetafunction* f)
	{
		lua_pushstring(L, name);
		lua_pushlightuserdata(L, f);
		lua_pushcclosure(L, [](lua_State* L)->int { 
											auto m = (GMetafunction*)lua_touserdata(L, lua_upvalueindex(1));
											m->invoke_lua(L); return 0; }
		, 1);

		//lua_pushcfunction(L, [](lua_State* L)->int { std::cout << "I am from CPP lambda!\n"; return 0; });
		lua_settable(L, -3);
	}
}
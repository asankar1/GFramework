#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <mutex>
#include <GFramework/GScript/GFrameworkLua.h>
#include <GFramework/GScript/GLua_data_exchange.h>
#include <GFramework/GVariant/GVariant.h>

namespace GFramework
{
	class GMetaclass;
	
	class GFRAMEWORK_API GLuaState
	{
	public:
		GLuaState();
		void add_namespace(const char* name);
		void add_constructors_metatable();
		void create_metatable(const char* name);
		void add_integer_to_metatable(const char* mt_name, const char* int_name, int value);
		void add_metatable_to_metatable(const char* mt_name, const char* tb_name, const char* value);
		void open_lua_classes_module();
		void register_script_for_metaclass(GMetaclass* metaclass);
		~GLuaState();
		lua_State* L;
	public:
		std::mutex GLuaMutex;
	private:
		
	};
	

	class GFRAMEWORK_API GLuaModule
	{
	public:
		GLuaModule(const char* _name);
		~GLuaModule();
		void add_namespace(const char* _name);
		const std::vector<luaL_Reg>& getTokensList() const;
		void exportToLua(lua_State *L);
	private:
		static int pushNamespaceToLua(lua_State* L);
	private:
		const char* name;
		std::set<const char*> namespaceList;
		std::vector<luaL_Reg> tokensList;
	};

	class GFRAMEWORK_API GLuaScript
	{
	public:
		virtual ~GLuaScript();
		virtual void printError(const std::string& variableName, const std::string& reason);
		virtual bool load() = 0;
		virtual bool run() = 0;
	protected:
		GLuaScript();

	protected:
		std::mutex luaStateMutex;
		lua_State* L;
	};

	class GFRAMEWORK_API GLuaScriptFile : public GLuaScript
	{
	public:
		GLuaScriptFile(const char * _file);
		~GLuaScriptFile();
		virtual bool load() override;
		virtual bool run() override;
	private:
		const char* file;
	};
	
	class GFRAMEWORK_API GLuaScriptChunk
	{
	public:
		GLuaScriptChunk(const std::string& script);
		void printError(const std::string& variableName, const std::string& reason);
		~GLuaScriptChunk();
	protected:
		const std::string chunk;
		lua_State* L;
	};

	/*void GFRAMEWORK_API add_constructors_metatable();
	void GFRAMEWORK_API create_metatable(const char* name);
	void GFRAMEWORK_API add_integer_to_metatable(const char* mt_name, const char* int_name, int value);
	void GFRAMEWORK_API open_lua_classes_module(lua_State* L);
	void GFRAMEWORK_API register_script_for_metaclass(GMetaclass* metaclass);*/
}


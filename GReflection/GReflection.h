#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <typeinfo>
#include <algorithm>
#include <type_traits>
#include <tuple>
#include <utility>

#include <boost\variant.hpp>
#include <boost\function.hpp>
#include <boost\bind.hpp>
#include <boost\any.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <GVariant.h>

#ifdef VARIANT_DYNAMIC_LIBRARY
#ifdef DLL_EXPORT
#define LIBRARY_API __declspec( dllexport )
#else
#define LIBRARY_API __declspec( dllimport )
#endif
#else
#define LIBRARY_API
#endif

#define META_FRIEND(c) friend struct c##_metacreator; friend GDerivedMetaclass<c>;
#define BEGIN_DEFINE_META(c) struct c##_metacreator	{ c##_metacreator() {
#define END_DEFINE_META(c) } }; static c##_metacreator _c##_metacreator;

/*! \file GReflection.h
*	\brief class, functions, enums, typedefs, macros and other definitions related to GReflection class.
*/

/* Requirements:
	> A way to create metaclass once per class
	> A metaclass may contain
		- Name of the class
		- List of metaproperties
		- List of metafunctions including constructors
		- A way to query all the metaclass data
		- A way to create an instance of the class
		- A way to serialize/deserialize the class

	> metaproperty may contain
		- Name of the property
		- Type of the property
		- Should be shown in the editor as properties
		- Should be exported to target application

	> metafunction may contain
		> Name of the function
		> return type
		> arguments count
		> arguments list with name and type

*/

namespace GFramework
{
	class GMetaclass;
	class GMetafunction;
	class GMetaconstructor;

	class LIBRARY_API GMetaconstructor
	{
	public:
		GMetaconstructor() {

		}
	private:
	};

	template<typename T>
	class LIBRARY_API GDerivedMetaconstructor : public GMetaconstructor
	{
	public:		
		GDerivedMetaconstructor() {

		}

		void invoke() {
			fptr();
		}
	private:
		T fptr;
	};
#if 1 // not working for generic case
	// to pass a vector of variant as arguments: try to use std::index_sequence and variadic template
	// to automatically find the type and cast from variant:

	template <typename FuncType>
	using Arity = boost::function_types::function_arity<FuncType>;

	template <typename FuncType>
	using ResultType = typename boost::function_types::result_type<FuncType>::type;

	template <typename FuncType, size_t ArgIndex>
	using ArgType = typename boost::mpl::at_c<boost::function_types::parameter_types<FuncType>, ArgIndex>::type;

	template <typename Func, typename IndexSeq>
	struct PrototypePrintHelper;

	template <typename Func, size_t... Inds>
	struct PrototypePrintHelper<Func, std::integer_sequence<size_t, Inds...> >
	{
		static std::string getPrototype()
		{
			std::string result("");			
			string typeNames[] = { typeid(ArgType<Func, Inds>).name()... };
			if (Arity<Func>::value > 1)
			{
				result.append(typeNames[1]);
			}
			for (int i = 2; i < Arity<Func>::value; ++i)
			{
				result.append(", ").append(typeNames[i]);
			}
			return result;
		}
	};

	class LIBRARY_API GMetaproperty
	{
	public:
		const char* getName() {
			return name.c_str();
		}

		virtual void set(void* _object, GVariant &value) = 0;
		virtual GVariant get(void* _object) = 0;
		virtual std::ostream& writeBinaryValue(std::ostream& os, Object* obj) = 0;
		virtual std::istream& readBinaryValue(std::istream& is, Object* obj) = 0;
		virtual std::ostream& writeASCIIValue(std::ostream& os, Object* obj) = 0;
		virtual std::istream& readASCIIValue(std::istream& is, Object* obj) = 0;
	protected:
		GMetaproperty(const char* _name) {
			name = std::string(_name);
		}
		std::string name;
	};

	template<typename C, typename T>
	class LIBRARY_API GDerivedMetaEditableproperty : public GMetaproperty
	{
	public:
		GDerivedMetaEditableproperty(const char* _name, T _ptr) : GMetaproperty(_name) {
			ptr = _ptr;
		}

		void set(void* _object, GVariant &value) {
			C* o = static_cast<C*>(_object);
			(o->*ptr).set(value);// = boost::get<T>(value);
		}

		GVariant get(void* _object) {
			C* o = static_cast<C*>(_object);
			return (o->*ptr).get();
		}

		virtual std::ostream& writeBinaryValue(std::ostream& os, Object* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).writeBinaryValue(os);
		}

		virtual std::istream& readBinaryValue(std::istream& is, Object* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readBinaryValue(is);
		}

		virtual std::ostream& writeASCIIValue(std::ostream& os, Object* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).writeASCIIValue(os);
		}

		virtual std::istream& readASCIIValue(std::istream& is, Object* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readASCIIValue(is);
		}
	private:
		T ptr;
	};

	template<typename C, typename T>
	class LIBRARY_API GDerivedMetaproperty : public GMetaproperty
	{
	public:
		GDerivedMetaproperty(const char* _name, T _ptr) : GMetaproperty(_name){
			ptr = _ptr;
		}

		void set(void* _object, GVariant &value) {
			C* o = static_cast<C*>(_object);
			(o->*ptr).set(value);// = boost::get<T>(value);
		}

		GVariant get(void* _object) {
			C* o = static_cast<C*>(_object);
			return (o->*ptr).get();
		}

		virtual std::ostream& writeBinaryValue(std::ostream& os, Object* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).writeBinaryValue(os);
		}

		virtual std::istream& readBinaryValue(std::istream& is, Object* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readBinaryValue(is);
		}

		virtual std::ostream& writeASCIIValue(std::ostream& os, Object* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).writeASCIIValue(os);
		}

		virtual std::istream& readASCIIValue(std::istream& is, Object* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readASCIIValue(is);
		}
	private:
		T ptr;
	};

	class LIBRARY_API GMetafunction
	{
	public:
		GMetafunction(const char *_name) : name(_name){
		}
		const std::string& getName() { return name; }
		const unsigned int getArgsCount() { return args.size();  }
		const std::list<std::string>& getArgsList() { return args; }
		virtual GVariant invoke(void * o, std::vector<GVariant>& args)=0 ;
		virtual std::string getPrototype() = 0;		
		virtual std::string getReturntype() = 0;
		virtual std::string getParameters() = 0;

	protected:
		std::list<std::string> args;
	private:
		std::string name;
	};

	// helper for functions taking non-void arguments and returning a non-void value
	template<typename R, typename C, typename F, int n>
	struct invoke_helper {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			result = call_internal(_object, f, args, std::make_index_sequence<n-1>());
			return result;
		}
	private:
		template<typename F, std::size_t... I>
		static GVariant call_internal(void* _object, F f, std::vector<GVariant>& args, std::index_sequence<I...>)
		{
			auto casted_object = static_cast<C*>(_object);
			return (casted_object->*f)(args[I]...);
		}
	};

	// helper for functions taking non-void arguments and returning a void value
	template<typename C, typename F, int n>
	struct invoke_helper<void, C, F, n> {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			call_internal(_object, f, args, std::make_index_sequence<n - 1>());
			return result;
		}
	private:
		template<typename F, std::size_t... I>
		static void call_internal(void* _object, F f, std::vector<GVariant>& args, std::index_sequence<I...>)
		{
			auto casted_object = static_cast<C*>(_object);
			(casted_object->*f)(args[I]...);
		}
	};

	// helper for functions taking void arguments and returning a non-void value
	template<typename R, typename C, typename F> 
	struct invoke_helper<R, C, F, 1> {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			auto casted_object = static_cast<C*>(_object);
			result = (casted_object->*f)();
			return result;
		}
	};

	// helper for functions taking void arguments and returning a void value
	template<typename C, typename F> //specialization for no parammeters function
	struct invoke_helper<void, C, F, 1> {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			auto casted_object = static_cast<C*>(_object);
			(casted_object->*f)();
			return result;
		}
	};

	template<typename C, typename F>
	class LIBRARY_API GDerivedMetafunction : public GMetafunction
	{
	public:
		GDerivedMetafunction(const char *_name, F _f) : GMetafunction(_name), f(_f) {
		}

		void arguments(){}
		virtual std::string getPrototype()
		{
			std::string return_type = getReturntype();
			std::string parameters = getParameters();
			std::string prototype = getReturntype();
			prototype = prototype.append(" ").append(getName()).append("(");
			prototype = prototype.append(parameters);
			prototype = prototype.append(")");
			//return PrototypePrintHelper<F, std::make_index_sequence<Arity<F>::value> >::getPrototype();
			return prototype;
		}

		virtual std::string getReturntype()
		{
			return typeid(ResultType<F>).name();
		}

		virtual std::string getParameters()
		{
			return PrototypePrintHelper<F, std::make_index_sequence<Arity<F>::value> >::getPrototype();
		}

		template<typename ARG1, typename ARG2, typename ARG3>
		void arguments() {
			args.push_back(typeid(T1).name());
			args.push_back(typeid(T2).name());
			args.push_back(typeid(T3).name());
		}
		
		GVariant invoke(void * _object, std::vector<GVariant>& _args) {
			GVariant result;
			result = invoke_helper<ResultType<F>, C, F, Arity<F>::value>::call(_object, f, _args);
			return result;
		}

	private:
		//boost::function<typename std::remove_reference<std::remove_const<decltype(*f)>::type>::type>  f;
		typename F f;
	};
#else
	class LIBRARY_API GMetafunction
	{
	public:
		typedef std::list<GVariant> returnType;
		typedef std::list<GVariant> argsType;
		GMetafunction(const char *_name) : name(_name) {
		}
		const std::string& getName() { return name; }
		const unsigned int getArgsCount() { return args.size(); }
		const std::list<std::string>& getArgsList() { return args; }
		virtual returnType invoke(void * _object, argsType& args_list) = 0;

	protected:
		std::list<std::string> args;

	private:
		std::string name;
	};

	template<typename C>
	class LIBRARY_API GDerivedMetafunction : public GMetafunction
	{
	public:
		GDerivedMetafunction(const char *_name, returnType(C::*_f)(argsType&)) : GMetafunction(_name), f(_f) {
		}
		GDerivedMetafunction<C>& tooltip(const char *_tooltip) {
			tooltipInfo = std::string(_tooltip);
			return *this;
		}
		const std::string& getName() { return name; }
		const unsigned int getArgsCount() { return args.size(); }
		const std::list<std::string>& getArgsList() { return args; }
		returnType invoke(void * _object, argsType& args_list) {
			float ff;
			auto casted_object = static_cast<C*>(_object);
			auto  a = boost::bind(f, casted_object, args_list);
			returnType r = a();
			return r;
		}
	private:
		std::string tooltipInfo;
		returnType(C::*f)(argsType&);
	};
#endif
	
	class LIBRARY_API GMetacluster
	{
	public:
		GMetacluster(){
		}

	protected:
		std::list<std::string> args;
	private:
		std::string name;
	};

	class GMetaclassList;
	class GMetaclass;
	template<typename T>
	class GDerivedMetaclass;
	template<typename T>
	class GDerivedAbstractMetaclass;

	class LIBRARY_API GMetaclassList
	{
	public:			

		template<typename T>
		auto& define(const char* _name)
		{
			typedef std::conditional<std::is_abstract<T>::value, GDerivedAbstractMetaclass<T>, GDerivedMetaclass<T>>::type MetaclassType;

			MetaclassType* m = MetaclassType::getInstance();
			if (m->name.empty())
			{
				m->name = std::string(_name);
				Gmetatable.insert(std::make_pair(_name, m));
				std::cout << "GMetaclass '" << _name << "' added to Metatable." << std::endl;
			}
			else
			{
				std::cout << "GMetaclass already exist under the name '" << m->name << "' in the Metatable." << std::endl;
			}
			return *m;
		}

		template<typename T>
		auto& getMetaclassByType()
		{
			auto m = std::conditional<std::is_abstract<T>::value, GDerivedAbstractMetaclass<T>, GDerivedMetaclass<T>>::type::getInstance();
			return *m;
		}

		GMetaclass* getMetaclass(const char* _name)
		{
			return Gmetatable[std::string(_name)];
		}
		static GMetaclassList& instance()
		{
			static GMetaclassList _o;
			return _o;
		}
		~GMetaclassList();
	private:
		GMetaclassList();

		std::map<std::string, GMetaclass*> Gmetatable;
	};

	class LIBRARY_API GMetaclass
	{
	public:
		const std::string& getName() { return name; }
		virtual Object* createInstance() = 0;
		virtual GMetafunction* getFunction(const char *_name)=0;
		virtual GMetaproperty* getProperty(const char *_name) = 0;
		void getFunctionsList(std::vector<std::string> &functions_list)
		{
			for (std::string baseclass : GBaseMetaclasses)
			{
				GMetaclass* basemetaclass = GMetaclassList::instance().getMetaclass(baseclass.c_str());
				if(basemetaclass)
					basemetaclass->getFunctionsList(functions_list);
			}
			for (auto it = Gmetafunctions.begin(); it != Gmetafunctions.end(); ++it) {
				functions_list.push_back(it->first);
			}
			return ;
		}

		void getPropertiesList(std::vector<std::string> &properties_list)
		{
			for (std::string baseclass : GBaseMetaclasses)
			{
				GMetaclass* basemetaclass = GMetaclassList::instance().getMetaclass(baseclass.c_str());
				if (basemetaclass)
					basemetaclass->getPropertiesList(properties_list);
			}
			for (auto it = Gmetaproperties.begin(); it != Gmetaproperties.end(); ++it) {
				properties_list.push_back(it->first);
			}
			return;
		}

		void getEditablePropertiesList(std::vector<std::string> &properties_list)
		{
			for (std::string baseclass : GBaseMetaclasses)
			{
				GMetaclass* basemetaclass = GMetaclassList::instance().getMetaclass(baseclass.c_str());
				if (basemetaclass)
					basemetaclass->getEditablePropertiesList(properties_list);
			}
			for (auto it = Gmetaeditableproperties.begin(); it != Gmetaeditableproperties.end(); ++it) {
				properties_list.push_back(it->first);
			}
			return;
		}

	protected:
		void addFunction(const char *_name, GMetafunction* _f);
		void addProperty(const char *_name, GMetaproperty* _p);
		void addEditableProperty(const char *_name, GMetaproperty* _p);
		GMetaclass();

	protected:		
		std::vector<std::string> GBaseMetaclasses;
		std::map<std::string, GMetafunction*> Gmetafunctions;
		std::map<std::string, GMetaproperty*> Gmetaproperties;
		std::map<std::string, GMetaproperty*> Gmetaeditableproperties;
		std::string name;
		friend GMetaclassList;
	};

	template<typename T>
	class LIBRARY_API GDerivedAbstractMetaclass : public GMetaclass
	{
	public:
		static GDerivedAbstractMetaclass<T>* getInstance()
		{
			static GDerivedAbstractMetaclass<T> t;
			return &t;
		}

		GDerivedAbstractMetaclass<T>& baseMetaclass(const char *_name)
		{
			if (std::find(GBaseMetaclasses.begin(), GBaseMetaclasses.end(), std::string(_name)) == GBaseMetaclasses.end())
			{
				GBaseMetaclasses.push_back(std::string(_name));
			}
			return *this;
		}

		//not applicable for abstract meta class. So assert if called.
		virtual Object* createInstance() {
			return nullptr;
		}

		template<typename T>
		GDerivedAbstractMetaclass<T>& constructor()
		{

			return *this;
		}

		template<typename PROP>
		GDerivedAbstractMetaclass<T>& editableProperty(const char* _name, PROP _p)
		{
			auto p = new GDerivedMetaEditableproperty<T, PROP>(_name, _p);
			addEditableProperty(_name, p);
			return *this;
		}

		template<typename PROP>
		GDerivedAbstractMetaclass<T>& property(const char* _name, PROP _p)
		{
			auto p = new GDerivedMetaproperty<T, PROP>(_name, _p);
			addProperty(_name, p);
			return *this;
		}

		template<typename FUNC>
		GDerivedAbstractMetaclass<T>& function(const char *_name, FUNC _f)
		{
			auto f = new GDerivedMetafunction<T, FUNC>(_name, _f);
			addFunction(_name, f);
			return *this;
		}
		GMetafunction* getFunction(const char *_name)
		{
			if (Gmetafunctions.find(std::string(_name)) != Gmetafunctions.end())
			{
				return Gmetafunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetafunction* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return NULL;
		}

		GMetaproperty* getProperty(const char *_name)
		{
			if (Gmetaeditableproperties.find(std::string(_name)) != Gmetaeditableproperties.end())
			{
				return Gmetaeditableproperties[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaproperty* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getProperty(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}

			if (Gmetaproperties.find(std::string(_name)) != Gmetaproperties.end())
			{
				return Gmetaproperties[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaproperty* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getProperty(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return NULL;
		}

	private:
		GDerivedAbstractMetaclass() {}
	};

	template<typename T>
	class LIBRARY_API GDerivedMetaclass : public GMetaclass
	{
	public:
		static GDerivedMetaclass<T>* getInstance()
		{
			static GDerivedMetaclass<T> t;
			return &t;
		}

		GDerivedMetaclass<T>& baseMetaclass(const char *_name)
		{
			if (std::find(GBaseMetaclasses.begin(), GBaseMetaclasses.end(), std::string(_name)) == GBaseMetaclasses.end())
			{
				GBaseMetaclasses.push_back(std::string(_name));
			}
			return *this;
		}

		virtual Object* createInstance() {
			return new T();
		}

		template<typename T>
		GDerivedMetaclass<T>& constructor()
		{

			return *this;
		}

		template<typename PROP>
		GDerivedMetaclass<T>& editableProperty(const char* _name, PROP _p)
		{
			auto p = new GDerivedMetaEditableproperty<T, PROP>(_name, _p);
			addEditableProperty(_name, p);
			return *this;
		}

		template<typename PROP>
		GDerivedMetaclass<T>& property(const char* _name, PROP _p)
		{
			auto p = new GDerivedMetaproperty<T, PROP>(_name, _p);
			addProperty(_name, p);
			return *this;
		}

		template<typename FUNC>
		GDerivedMetaclass<T>& function(const char *_name, FUNC _f)
		{
			auto f = new GDerivedMetafunction<T, FUNC>(_name, _f);
			addFunction(_name, f);
			return *this;
		}
		GMetafunction* getFunction(const char *_name)
		{
			if (Gmetafunctions.find(std::string(_name)) != Gmetafunctions.end())
			{
				return Gmetafunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetafunction* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return NULL;
		}

		GMetaproperty* getProperty(const char *_name)
		{
			if (Gmetaeditableproperties.find(std::string(_name)) != Gmetaeditableproperties.end())
			{
				return Gmetaeditableproperties[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaproperty* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getProperty(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}

			if (Gmetaproperties.find(std::string(_name)) != Gmetaproperties.end())
			{
				return Gmetaproperties[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaproperty* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getProperty(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return NULL;
		}

	private:
		GDerivedMetaclass() {}
	};

	class LIBRARY_API GReflection
	{
	public:
		GReflection();
		~GReflection();
	};
}

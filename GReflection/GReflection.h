#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <typeinfo>
#include <algorithm>
#include <type_traits>
#include <tuple>
#include <utility>

#include <boost/variant.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <GVariant/GVariant.h>
#include <GScript/GLua_data_exchange.h>
#include <GScript/GLuaScript.h>

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
#define END_DEFINE_META(c) register_script_for_metaclass(GMetaclassList::instance().getMetaclassByType<c>());} }; static c##_metacreator _c##_metacreator;

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
	class GMetaMemberfunction;
	class GMetaconstructor;

	typedef GMetafunction GMetaStaticfunction;

	class GMetaconstructor
	{
	public:
		GMetaconstructor() {

		}
	private:
	};

	template<typename T>
	class GDerivedMetaconstructor : public GMetaconstructor
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
		static std::string getMemberFunctionPrototype()
		{
			std::string result("");
			std::string typeNames[] = { typeid(ArgType<Func, Inds>).name()... };
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

		static std::string getStaticFunctionPrototype()
		{
			std::string result("");
			std::string typeNames[] = { "", typeid(ArgType<Func, Inds>).name()... };
			if (Arity<Func>::value > 0)
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

	class GMetaproperty
	{
	public:
		const char* getName() {
			return name.c_str();
		}

		virtual void set(void* _object, GVariant &value) = 0;
		virtual GVariant get(void* _object) = 0;
		virtual std::ostream& writeBinaryValue(std::ostream& os, const GObject* obj) = 0;
		virtual std::istream& readBinaryValue(std::istream& is, GObject* obj) = 0;
		virtual std::ostream& writeASCIIValue(std::ostream& os, const GObject* obj) = 0;
		virtual std::istream& readASCIIValue(std::istream& is, GObject* obj) = 0;
	protected:
		GMetaproperty(const char* _name) {
			name = std::string(_name);
		}
		std::string name;
	};

	template<typename C, typename T>
	class GDerivedMetaEditableproperty : public GMetaproperty
	{
	public:
		GDerivedMetaEditableproperty(const char* _name, T _ptr, std::function<void(C*)> callback) : GMetaproperty(_name) {
			ptr = _ptr;
			onupdate_cb = callback;
		}

		void set(void* _object, GVariant &value) {
			C* o = static_cast<C*>(_object);
			(o->*ptr).set(value);// = boost::get<T>(value);
			if (onupdate_cb) {
				onupdate_cb(o);
			}
		}

		GVariant get(void* _object) {
			C* o = static_cast<C*>(_object);
			return (o->*ptr).get();
		}

		virtual std::ostream& writeBinaryValue(std::ostream& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			return (o->*ptr).writeBinaryValue(os);
		}

		virtual std::istream& readBinaryValue(std::istream& is, GObject* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readBinaryValue(is);
		}

		virtual std::ostream& writeASCIIValue(std::ostream& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			return (o->*ptr).writeASCIIValue(os);
		}

		virtual std::istream& readASCIIValue(std::istream& is, GObject* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readASCIIValue(is);
		}
	private:
		T ptr;
		std::function<void(C*)> onupdate_cb;
	};

	template<typename C, typename T>
	class GDerivedMetaproperty : public GMetaproperty
	{
	public:
		GDerivedMetaproperty(const char* _name, T _ptr) : GMetaproperty(_name) {
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

		virtual std::ostream& writeBinaryValue(std::ostream& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			return (o->*ptr).writeBinaryValue(os);
		}

		virtual std::istream& readBinaryValue(std::istream& is, GObject* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readBinaryValue(is);
		}

		virtual std::ostream& writeASCIIValue(std::ostream& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			return (o->*ptr).writeASCIIValue(os);
		}

		virtual std::istream& readASCIIValue(std::istream& is, GObject* obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readASCIIValue(is);
		}
	private:
		T ptr;
	};

	// helper for functions taking non-void arguments and returning a non-void value
	template<typename R, typename C, typename F, int n>
	struct member_function_invoke_helper {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			result = call_internal(_object, f, args, std::make_index_sequence<n - 1>());
			return result;
		}

	private:
		template<typename F2, std::size_t... I>
		static GVariant call_internal(void* _object, F2 f, std::vector<GVariant>& args, std::index_sequence<I...>)
		{
			auto casted_object = static_cast<C*>(_object);
			return (casted_object->*f)(GVariant::cast<ArgType<F2, I + 1>>(args[I])...);
		}
	};

	// helper for functions taking non-void arguments and returning a void value
	template<typename C, typename F, int n>
	struct member_function_invoke_helper<void, C, F, n> {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			call_internal(_object, f, args, std::make_index_sequence<n - 1>());
			return result;
		}

	private:
		template<typename F2, std::size_t... I>
		static void call_internal(void* _object, F2 f, std::vector<GVariant>& args, std::index_sequence<I...>)
		{
			auto casted_object = static_cast<C*>(_object);
			(casted_object->*f)(GVariant::cast<ArgType<F2, I + 1>>(args[I])...);
		}
	};

	//helper for creating GVariant if return type is non reference
	template<typename R, typename C, typename F, bool b>
	struct member_function_invoke_helper_returns_reference
	{
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			auto casted_object = static_cast<C*>(_object);
			result = (casted_object->*f)();
			return result;
		}
	};

	//helper for creating GVariant if return type is reference
	template<typename R, typename C, typename F>
	struct member_function_invoke_helper_returns_reference<R, C, F, true> {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			auto casted_object = static_cast<C*>(_object);
			typedef typename std::remove_reference<R>::type NON_REFERENCE_TYPE;
			result = GVariant::ref<NON_REFERENCE_TYPE>((casted_object->*f)());
			return result;
		}
	};

	// helper for functions taking void arguments and returning a non-void value
	template<typename R, typename C, typename F>
	struct member_function_invoke_helper<R, C, F, 1> {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			//auto casted_object = static_cast<C*>(_object);
			//result = (casted_object->*f)();
			result = member_function_invoke_helper_returns_reference < R, C, F, std::is_reference<R>::value >::call(_object, f, args);
			//result = std::conditional<std::is_reference<R>::value, GVariant::ref<std::remove_reference<F>::type>((casted_object->*f)()), (casted_object->*f)();
			return result;
		}
	};

	// helper for functions taking void arguments and returning a void value
	template<typename C, typename F> //specialization for no parammeters function
	struct member_function_invoke_helper<void, C, F, 1> {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			auto casted_object = static_cast<C*>(_object);
			(casted_object->*f)();
			return result;
		}
	};

	//static function invoke helpers
	// helper for functions taking non-void arguments and returning a non-void value
	template<typename R, typename C, typename F, int n>
	struct nonmember_function_invoke_helper {
		static GVariant static_call(F f, std::vector<GVariant>& args)
		{
			GVariant result;
			result = static_call_internal(f, args, std::make_index_sequence<n>());
			return result;
		}
	private:
		template<typename F2, std::size_t... I>
		static GVariant static_call_internal(F2 f, std::vector<GVariant>& args, std::index_sequence<I...>)
		{
			return (*f)(GVariant::cast<ArgType<F2, I>>(args[I])...);
		}
	};

	// helper for functions taking non-void arguments and returning a void value
	template<typename C, typename F, int n>
	struct nonmember_function_invoke_helper<void, C, F, n> {
		static GVariant static_call(F f, std::vector<GVariant>& args)
		{
			GVariant result;
			static_call_internal(f, args, std::make_index_sequence<n>());
			return result;
		}
	private:
		template<typename F2, std::size_t... I>
		static void static_call_internal(F2 f, std::vector<GVariant>& args, std::index_sequence<I...>)
		{
			(*f)(GVariant::cast<ArgType<F2, I>>(args[I])...);
		}
	};

	class GFRAMEWORK_API GMetafunction_Base
	{
	public:
		GMetafunction_Base(const char *_name) : name(_name) {
		}
		const std::string& getName() { return name; }
		//const unsigned int getArgsCount() { return args.size();  }
		//const std::list<std::string>& getArgsList() { return args; }
		virtual int invoke_lua(lua_State*L) = 0;
		//virtual std::string getPrototype() = 0;		
		virtual std::string getReturntype() = 0;
		virtual std::string getParameters() = 0;

		virtual std::string getPrototype()
		{
			std::string parameters = getParameters();
			std::string prototype = getReturntype();
			prototype = prototype.append(" ").append(getName()).append("(");
			prototype = prototype.append(parameters);
			prototype = prototype.append(")");
			//return PrototypePrintHelper<F, std::make_index_sequence<Arity<F>::value> >::getPrototype();
			return prototype;
		}
	protected:
		template<typename first>
		static void iterate_arg_type(lua_State* L, std::vector<GVariant>& args, int offset)
		{
			//cout << typeid(first).name() << endl;
			Glua_puller< typename std::decay<first>::type >(L, args, offset);
		}

		/*template<>
		static void iterate_arg_type<void>(lua_State* L, std::vector<GVariant>& args, int offset)
		{
		//TODO: see if this specialization can be removed
		//cout << typeid(first).name() << endl;
		//do nothing
		}*/

		template<typename first, typename second, typename... rest>
		static void iterate_arg_type(lua_State* L, std::vector<GVariant>& args, int offset)
		{
			//cout << typeid(first).name() << endl;
			Glua_puller< typename std::decay<first>::type >(L, args, offset);
			iterate_arg_type<second, rest...>(L, args, offset);
		}

		template<class R, class C, class... Args>
		static void proto(R(C::*)(Args...), lua_State* L, std::vector<GVariant>& args, int offset)
		{
			GMetafunction_Base::iterate_arg_type<Args...>(L, args, offset);
		}

		template<class R, class C, class... Args>
		static void proto(R(C::*)(Args...)const, lua_State* L, std::vector<GVariant>& args, int offset)
		{
			GMetafunction_Base::iterate_arg_type<Args...>(L, args, offset);
		}

		template<class R, class... Args>
		static void proto(R(*)(Args...), lua_State* L, std::vector<GVariant>& args, int offset)
		{
			GMetafunction_Base::iterate_arg_type<Args...>(L, args, offset);
		}

		template<typename F>
		class non_void {
		public:
			static void pass(F f, lua_State* L, std::vector<GVariant>& args, int offset) {
				proto(f, L, args, offset);
			}
		};

		template<typename F>
		class yes_void {
		public:
			static void pass(F f, lua_State* L, std::vector<GVariant>& args, int offset) {

			}
		};

		template<class F>
		struct function_traits;

		template<class R, class... Args>
		struct function_traits<R(Args...)>
		{
			using return_type = R;

			static constexpr std::size_t arity = sizeof...(Args);

			template <std::size_t N>
			struct argument
			{
				static_assert(N < arity, "error: invalid parameter index.");
				using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
			};
		};

		template<class R, class... Args>
		struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)>
		{};

		// function pointer
		template<class R, class C, class... Args>
		struct function_traits<R(C::*)(Args...)const> : public function_traits<R(Args...)>
		{};

		template<class R, class C, class... Args>
		struct function_traits<R(C::*)(Args...)> : public function_traits<R(Args...)>
		{};


		//TODO: Try to avoid the offset parameter and handle member function and non member fumction arguments efficiently
		template <typename F>
		void fill_args_from_lua(F f, lua_State* L, std::vector<GVariant>& args, int offset)
		{
			typedef typename std::conditional<function_traits<F>::arity, non_void<F>, yes_void<F> >::type selection;
			//std::cout << "boost arity for " << getName().c_str() << " is " << Arity<F>::value << std::endl;
			//std::cout << "simple arity for " << getName().c_str() << " is " << function_traits<F>::arity << std::endl;
			selection::pass(f, L, args, offset);
			//iterate_arg_type<Args...>();
		}

	private:
		std::string name;
	};

	template<>
	void GMetafunction_Base::iterate_arg_type<void>(lua_State* L, std::vector<GVariant>& args, int offset);

	class GMetaMemberfunction : public GMetafunction_Base
	{
	public:
		GMetaMemberfunction(const char *_name) : GMetafunction_Base(_name) {
		}
		virtual GVariant invoke(void * o, std::vector<GVariant>& args) = 0;
	};

	template<typename C, typename F>
	class GMetaMemberfunction_derived : public GMetaMemberfunction
	{
	public:
		GMetaMemberfunction_derived(const char *_name, F _f) : GMetaMemberfunction(_name), func(_f) {
		}

		virtual std::string getReturntype()
		{
			return typeid(ResultType<F>).name();
		}

		virtual std::string getParameters()
		{
			return PrototypePrintHelper<F, std::make_index_sequence<Arity<F>::value> >::getMemberFunctionPrototype();
		}

		virtual GVariant invoke(void * _object, std::vector<GVariant>& _args) {
			GVariant result;
			result = member_function_invoke_helper<ResultType<F>, C, F, Arity<F>::value>::call(_object, func, _args);
			return result;
		}

		virtual int invoke_lua(lua_State* L)
		{
			Glua_abstract_ptr* userdata = (Glua_abstract_ptr*)lua_touserdata(L, 1);
			std::vector<GVariant> args;
			fill_args_from_lua(func, L, args, 2);
			GVariant rv = invoke(userdata->object, args);
			Glua_pusher< typename std::decay<ResultType<F> >::type >(L, rv);
			return 1;
		}
	private:
		//boost::function<typename std::remove_reference<std::remove_const<decltype(*f)>::type>::type>  f;
		F func;
	};

	class GMetafunction : public GMetafunction_Base
	{
	public:
		GMetafunction(const char *_name) : GMetafunction_Base(_name) {
		}
		virtual GVariant invoke(std::vector<GVariant>& _args) = 0;
	};

	template<typename C, typename F>
	class GMetafunction_derived : public GMetafunction
	{
	public:
		GMetafunction_derived(const char *_name, F _f) : GMetafunction(_name), func(_f) {
		}

		virtual std::string getReturntype()
		{
			return typeid(ResultType<F>).name();
		}

		virtual std::string getParameters()
		{
			return PrototypePrintHelper<F, std::make_index_sequence<Arity<F>::value> >::getStaticFunctionPrototype();
		}

		virtual GVariant invoke(std::vector<GVariant>& _args) override
		{
			GVariant result;
			result = nonmember_function_invoke_helper<ResultType<F>, C, F, Arity<F>::value>::static_call(func, _args);
			return result;
		}

		virtual int invoke_lua(lua_State* L) override
		{
			std::vector<GVariant> args;
			fill_args_from_lua(func, L, args, 1);
			GVariant rv = invoke(args);
			Glua_pusher< typename std::decay<ResultType<F> >::type >(L, rv);
			//Glua_pusher< std::remove_cv< ResultType<F> > >(L, rv);
			return 1;
		}

	private:
		//boost::function<typename std::remove_reference<std::remove_const<decltype(*f)>::type>::type>  f;
		F func;
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
	class LIBRARY_API GMetaMemberfunction : public GMetafunction
	{
	public:
		GMetaMemberfunction(const char *_name, returnType(C::*_f)(argsType&)) : GMetafunction(_name), f(_f) {
		}
		GMetaMemberfunction<C>& tooltip(const char *_tooltip) {
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

	/*class GMetacluster
	{
	public:
		GMetacluster() {
		}

	protected:
		std::list<std::string> args;
	private:
		std::string name;
	};*/

	class GMetaclassList;
	class GMetaclass;
	template<typename T>
	class GDerivedMetaclass;
	template<typename T>
	class GDerivedAbstractMetaclass;

	class GFRAMEWORK_API GMetaclassList
	{
	public:

		template<typename T>
		auto& define(const char* _name)
		{
			typedef typename std::conditional<std::is_abstract<T>::value, GDerivedAbstractMetaclass<T>, GDerivedMetaclass<T>>::type MetaclassType;

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
		auto getMetaclassByType()
		{
			auto m = std::conditional<std::is_abstract<T>::value, GDerivedAbstractMetaclass<T>, GDerivedMetaclass<T>>::type::getInstance();
			return m;
		}

		GMetaclass* getMetaclass(const char* _name)
		{
			auto itr = Gmetatable.find(std::string(_name));
			if (itr == Gmetatable.end()) {
				return nullptr;
			}
			return itr->second;
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

	class GFRAMEWORK_API GMetaclass
	{
	public:
		const std::string& getName() { return name; }
		virtual GObject* createInstance() = 0;
		virtual GMetaMemberfunction* getMemberFunction(const char *_name) = 0;
		virtual GMetaStaticfunction* getStaticFunction(const char *_name) = 0;
		virtual GMetaproperty* getProperty(const char *_name) = 0;
		void getFunctionsList(std::vector<std::string> &functions_list)
		{
			for (std::string baseclass : GBaseMetaclasses)
			{
				GMetaclass* basemetaclass = GMetaclassList::instance().getMetaclass(baseclass.c_str());
				if (basemetaclass)
					basemetaclass->getFunctionsList(functions_list);
			}
			for (auto it = Gmetamemberfunctions.begin(); it != Gmetamemberfunctions.end(); ++it) {
				functions_list.push_back(it->first);
			}
			return;
		}

		void getStaticFunctionsList(std::vector<std::string> &functions_list)
		{
			for (std::string baseclass : GBaseMetaclasses)
			{
				GMetaclass* basemetaclass = GMetaclassList::instance().getMetaclass(baseclass.c_str());
				if (basemetaclass)
					basemetaclass->getStaticFunctionsList(functions_list);
			}
			for (auto it = GmetaStaticfunctions.begin(); it != GmetaStaticfunctions.end(); ++it) {
				functions_list.push_back(it->first);
			}
			return;
		}

		void getPropertiesList(std::vector<std::string> &properties_list)
		{
			for (std::string baseclass : GBaseMetaclasses)
			{
				GMetaclass* basemetaclass = GMetaclassList::instance().getMetaclass(baseclass.c_str());
				if (basemetaclass)
					basemetaclass->getPropertiesList(properties_list);
			}
			for (auto it1 = Gmetaproperties.begin(); it1 != Gmetaproperties.end(); ++it1) {
				for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
					properties_list.push_back(it2->first);
				}
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
			for (auto it1 = Gmetaeditableproperties.begin(); it1 != Gmetaeditableproperties.end(); ++it1) {
				for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
					properties_list.push_back(it2->first);
				}
			}
			return;
		}

		unsigned int getVersion()
		{
			return properties_version;
		}

	protected:
		void addFunction(const char *_name, GMetaMemberfunction* _f);
		void addStaticFunction(const char *_name, GMetaStaticfunction* _f);
		void addProperty(const char *_name, GMetaproperty* _p);
		void addEditableProperty(const char *_name, GMetaproperty* _p);
		GMetaclass();

	protected:
		std::vector<std::string> GBaseMetaclasses;
		std::unordered_map<std::string, GMetaMemberfunction*> Gmetamemberfunctions;
		std::unordered_map<std::string, GMetaStaticfunction*> GmetaStaticfunctions;
		std::unordered_map<unsigned int, std::unordered_map<std::string, GMetaproperty*>> Gmetaproperties;
		std::unordered_map<unsigned int, std::unordered_map<std::string, GMetaproperty*>> Gmetaeditableproperties;
		std::string name;
		friend GMetaclassList;
		unsigned int properties_version;
	};

	template<typename T>
	class GDerivedAbstractMetaclass : public GMetaclass
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
		virtual GObject* createInstance() {
			return nullptr;
		}

		template<typename T2>
		GDerivedAbstractMetaclass<T2>& constructor()
		{
			return *this;
		}

		template<typename PROP>
		GDerivedAbstractMetaclass<T>& editableProperty(const char* _name, PROP _p, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GDerivedMetaEditableproperty<T, PROP>(_name, _p, callback);
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
			auto f = new GMetaMemberfunction_derived<T, FUNC>(_name, _f);
			addFunction(_name, f);
			return *this;
		}

		template<typename FUNC>
		GDerivedAbstractMetaclass<T>& staticFunction(const char *_name, FUNC _f)
		{
			auto f = new GMetafunction_derived<T, FUNC>(_name, _f);
			addStaticFunction(_name, f);
			return *this;
		}

		GDerivedAbstractMetaclass<T>& version(unsigned int v)
		{
			properties_version = v;
			return *this;
		}

		GMetaMemberfunction* getMemberFunction(const char *_name)
		{
			if (Gmetamemberfunctions.find(std::string(_name)) != Gmetamemberfunctions.end())
			{
				return Gmetamemberfunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaMemberfunction* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getMemberFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return NULL;
		}

		GMetaStaticfunction* getStaticFunction(const char *_name)
		{
			if (GmetaStaticfunctions.find(std::string(_name)) != GmetaStaticfunctions.end())
			{
				return GmetaStaticfunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaStaticfunction* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getStaticFunction(_name);
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
			std::unordered_map<unsigned int, std::unordered_map<std::string, GMetaproperty*>>::iterator it1 = Gmetaeditableproperties.begin();
			for (; it1 != Gmetaeditableproperties.end(); ++it1) {
				std::unordered_map<std::string, GMetaproperty*>::iterator result = it1->second.find(std::string(_name));
				if (result != it1->second.end())
				{
					return result->second;
				}
			}

			for (auto basemetaclass : GBaseMetaclasses)
			{
				GMetaproperty* p = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getProperty(_name);
				if (p != NULL)
				{
					return p;
				}
			}

			for (auto it1 = Gmetaproperties.begin(); it1 != Gmetaproperties.end(); ++it1) {
				auto result = it1->second.find(std::string(_name));
				if (result != it1->second.end())
				{
					return result->second;
				}
			}

			for (auto basemetaclass : GBaseMetaclasses)
			{
				GMetaproperty* p = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getProperty(_name);
				if (p != NULL)
				{
					return p;
				}
			}
			return NULL;
		}

	private:
		GDerivedAbstractMetaclass() {}
	};

	template<typename T>
	class GDerivedMetaclass : public GMetaclass
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

		virtual GObject* createInstance() {
			return new T();
		}

		template<typename T2>
		GDerivedMetaclass<T2>& constructor()
		{
			return *this;
		}

		template<typename PROP>
		GDerivedMetaclass<T>& editableProperty(const char* _name, PROP _p, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GDerivedMetaEditableproperty<T, PROP>(_name, _p, callback);
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

		GDerivedMetaclass<T>& version(unsigned int v)
		{
			properties_version = v;
			return *this;
		}

		template<typename FUNC>
		GDerivedMetaclass<T>& function(const char *_name, FUNC _f)
		{
			auto f = new GMetaMemberfunction_derived<T, FUNC>(_name, _f);
			addFunction(_name, f);
			return *this;
		}

		GMetaMemberfunction* getMemberFunction(const char *_name)
		{
			if (Gmetamemberfunctions.find(std::string(_name)) != Gmetamemberfunctions.end())
			{
				return Gmetamemberfunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaMemberfunction* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getMemberFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return NULL;
		}

		GMetaStaticfunction* getStaticFunction(const char *_name)
		{
			if (GmetaStaticfunctions.find(std::string(_name)) != GmetaStaticfunctions.end())
			{
				return GmetaStaticfunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaStaticfunction* f = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getStaticFunction(_name);
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
			std::unordered_map<unsigned int, std::unordered_map<std::string, GMetaproperty*>>::iterator it1 = Gmetaeditableproperties.begin();
			for (; it1 != Gmetaeditableproperties.end(); ++it1) {
				std::unordered_map<std::string, GMetaproperty*>::iterator result = it1->second.find(std::string(_name));
				if (result != it1->second.end())
				{
					return result->second;
				}
			}

			for (auto basemetaclass : GBaseMetaclasses)
			{
				GMetaproperty* p = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getProperty(_name);
				if (p != NULL)
				{
					return p;
				}
			}

			for (auto it1 = Gmetaproperties.begin(); it1 != Gmetaproperties.end(); ++it1) {
				auto result = it1->second.find(std::string(_name));
				if (result != it1->second.end())
				{
					return result->second;
				}
			}

			for (auto basemetaclass : GBaseMetaclasses)
			{
				GMetaproperty* p = GMetaclassList::instance().getMetaclass(basemetaclass.c_str())->getProperty(_name);
				if (p != NULL)
				{
					return p;
				}
			}
			return NULL;
		}

	private:
		GDerivedMetaclass() {}
	};

	class GFRAMEWORK_API GReflection
	{
	public:
		GReflection();
		~GReflection();
	};
}

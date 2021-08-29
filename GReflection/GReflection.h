#pragma once
#include <typeinfo>
#include <typeindex>
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

#ifndef Q_MOC_RUN
#include <boost/variant.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#endif
#include <GFramework/GVariant/GVariant.h>
#include <GFramework/GVariant/GProperty.h>
#include <GFramework/GSerialization/GSerializer.h>
#include <GFramework/GReflection/GReflectionHelpers.h>
//#include <GFramework/GScript/GLua_data_exchange.h>
//#include <GFramework/GScript/GLuaScript.h>
#include <GFramework/GScript/GScript.h>

#define DECLARE_META_CLASS(c) class c##_metacreator; /*friend GMetaNonAbstractclass<c>;*/
#define DECLARE_META_FRIEND(c) friend c##_metacreator; /*friend GMetaNonAbstractclass<c>;*/
#define BEGIN_DEFINE_META(c) struct c##_metacreator	{ GMetaclass* m; c##_metacreator() { m =&
#define END_DEFINE_META(c) } }; static const c##_metacreator c##_metacreator_;
#define GET_METACLASS_INTERNAL(c) c##_metacreator_.m

#define BEGIN_DEFINE_META_FUNCTION(n) struct n##_metacreator	{ GMetaNamespace* m; n##_metacreator() { m =&
#define END_DEFINE_META_FUNCTION(n) } }; static const n##_metacreator n##_metacreator_;
//#define END_DEFINE_META(c) GLuaState::register_script_for_metaclass(GMetaclassList::instance().getMetaclassByType<c>());} }; static c##_metacreator _c##_metacreator;

//#define IS_BASE(BASE_TYPE, DERIVED_TYPE) std::is_base_of<BASE_TYPE, DERIVED_TYPE>::value
//#define IS_SHARED_PTR(TYPE) is_shared_ptr<TYPE>::value
//#define CONDITIONAL_TYPE(CONDITION, YES_TYPE, NO_TYPE) typename std::conditional<CONDITION, YES_TYPE,NO_TYPE>::type

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
	class GMetaNamespaceList;
	class GMetaNamespace;
	class GMetaclass;
	//class GMetaclassList;
	template<typename T>
	class GMetaNonAbstractclass;
	template<typename T>
	class GMetaAbstractclass;
	class GMetafunction;
	class GMetaMemberfunction;
	class GMetaconstructor;
	class GMetaproperty;
	class GSerializer;
	class GDeserializer;

	typedef GMetafunction GMetaStaticfunction;

#if 0
	class GFRAMEWORK_API GMetaclassList
	{
	public:

		template<typename T>
		auto& define(const char* _name)
		{
			typedef typename std::conditional<std::is_abstract<T>::value, GMetaAbstractclass<T>, GMetaNonAbstractclass<T>>::type MetaclassType;

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
			auto m = std::conditional<std::is_abstract<T>::value, GMetaAbstractclass<T>, GMetaNonAbstractclass<T>>::type::getInstance();
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

		/*static GMetaclassList& instance()
		{
			static GMetaclassList _o;
			return _o;
		}*/
		void print(std::string indent = "");
		~GMetaclassList();
	private:
		GMetaclassList();
		friend class GMetaNamespace;
		std::map<std::string, GMetaclass*> Gmetatable;
	};
#endif
	template<class T>
	struct is_shared_ptr : std::false_type {};

	template<class T>
	struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};


	template<typename C, typename T>
	struct class_member_var
	{
		typedef C class_type;
		typedef T member_var_type;
	};

	template<typename C, typename T>
	class_member_var<C, T> find_type(T C::* p) {
		return class_member_var<C, T>();
	}

	class GMetaproperty
	{
	public:
		const char* getName() {
			return name.c_str();
		}
		virtual std::type_index getTypeInfo() = 0;
		virtual bool isGObjectPointer() = 0;
		virtual std::type_index getPointedGObjectTypeIndex() = 0;
		virtual void set(void* _object, GVariant& value) = 0;
		virtual GVariant get(void* _object) = 0;
		virtual GSerializer& writeBinaryValue(GSerializer& os, const GObject* obj) = 0;
		virtual GDeserializer& readBinaryValue(GDeserializer& is, GObjectSharedPtr obj) = 0;
		virtual GSerializer& writeASCIIValue(GSerializer& os, const GObject* obj) = 0;
		virtual GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj) = 0;
		virtual void write(GSerializer* os, const GObject* obj) { }
		virtual void setGObjectSharedPointer(GObject* obj, std::shared_ptr<GObject>& value) {}
	protected:
		GMetaproperty(const char* _name) {
			name = std::string(_name);
		}
		std::string name;
	};
#if 0
	template<typename C, typename T>
	class GDerivedMetaEditableproperty : public GMetaproperty
	{
	public:
		GDerivedMetaEditableproperty(const char* _name, T _ptr, std::function<void(C*)> callback) : GMetaproperty(_name) {
			ptr = _ptr;
			onupdate_cb = callback;
		}

		std::type_index getTypeInfo() {
			return typeid(T);
		}

		virtual bool isGObjectPointer() {
			//TODO: avoid find_type function invoke
			auto t = find_type(ptr);
			using type = decltype(t);
			using mem_type = type::member_var_type;
			return mem_type().isGObjectPointer();
		}

		virtual std::type_index getPointedGObjectTypeIndex() {
			//TODO: avoid find_type function invoke
			auto t = find_type(ptr);
			using type = decltype(t);
			using mem_type = type::member_var_type;
			return mem_type().getPointedGObjectTypeIndex();
		}

		void set(void* _object, GVariant& value) {
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

		virtual GSerializer& writeBinaryValue(GSerializer& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			return (o->*ptr).writeBinaryValue(os);
		}

		virtual GDeserializer& readBinaryValue(GDeserializer& is, GObjectSharedPtr obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readBinaryValue(is);
		}

		virtual GSerializer& writeASCIIValue(GSerializer& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			return (o->*ptr).writeASCIIValue(os);
		}

		virtual GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj) {
			C* o = static_cast<C*>(obj);
			return (o->*ptr).readASCIIValue(is);
		}
	private:
		T ptr;
		std::function<void(C*)> onupdate_cb;
	};

	template<typename C, typename GETTER_F, typename SETTER_F>
	class GDerivedNonPublicMetaEditableproperty : public GMetaproperty
	{
	public:
		GDerivedNonPublicMetaEditableproperty(const char* _name, GETTER_F get, SETTER_F set, std::function<void(C*)> callback) : GMetaproperty(_name) {
			getter = get;
			setter = set;
			onupdate_cb = callback;
		}

		std::type_index getTypeInfo() {
			return typeid(ResultType<GETTER_F>);
		}

		virtual bool isGObjectPointer() {
			//TODO: avoid find_type function invoke
			using mem_type = std::remove_pointer<ResultType<GETTER_F> >::type;
			if (std::is_pointer<mem_type>::value)
			{
				if (std::is_base_of<GObject, std::remove_pointer<mem_type>::type >::value)
				{
					return true;
				}
			}
			return false;
		}

		virtual std::type_index getPointedGObjectTypeIndex() {
			//TODO: avoid find_type function invoke
			using type = ResultType<GETTER_F>;
			return type_index(typeid(type));
		}

		void set(void* _object, GVariant& value) {
			C* o = static_cast<C*>(_object);
			//(o->*ptr).set(value);// = boost::get<T>(value);
			(o->*setter)(GVariant::cast<ArgType<SETTER_F, 1>>(value));
			if (onupdate_cb) {
				onupdate_cb(o);
			}
		}

		GVariant get(void* _object) {
			C* o = static_cast<C*>(_object);
			return (o->*getter)();
		}

		virtual GSerializer& writeBinaryValue(GSerializer& os, const GObject* obj) {
			//TODO: Write a Text/Binary deser for non GProperty
			return os;
		}

		virtual GDeserializer& readBinaryValue(GDeserializer& is, GObjectSharedPtr obj) {
			//TODO: Write a Text/Binary deser for non GProperty
			return is;
		}

		virtual GSerializer& writeASCIIValue(GSerializer& os, const GObject* obj) {
			//TODO: Write a Text/Binary deser for non GProperty
			const C* o = static_cast<const C*>(obj);
			return GPropertyInterface::ToProperty((o->*getter)())->writeASCIIValue(os);
			//return (o->*getter)().writeASCIIValue(os);
			//return (o->*ptr).writeASCIIValue(os);
			//return os;
		}

		virtual GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj) {
			//TODO: Write a Text/Binary deser for non GProperty
			/*C* o = static_cast<C*>(obj);
			std::remove_reference<ArgType<SETTER_F, 1> >::type prop;
			prop.readASCIIValue(is);
			(o->*setter)(prop);*/
			return is;
		}
	private:
	private:
		GETTER_F getter;
		SETTER_F setter;
		std::function<void(C*)> onupdate_cb;
	};
#endif	
	//for public property (without getter and setter)
	template<typename C, typename T>
	class GPublicMetaproperty : public GMetaproperty
	{
	public:
		GPublicMetaproperty(const char* _name, T _ptr, std::function<void(C*)> callback) : GMetaproperty(_name), ptr(_ptr), onupdate_cb(callback) {

		}

		std::type_index getTypeInfo() {
			return typeid(T);
		}

		virtual bool isGObjectPointer() {
			//TODO: avoid find_type function invoke
			auto t = find_type(ptr);
			using type = decltype(t);
			using mem_type = typename type::member_var_type;
			//return mem_type().isGObjectPointer();
			return false;
		}

		virtual std::type_index getPointedGObjectTypeIndex() {
			//TODO: avoid find_type function invoke
			auto t = find_type(ptr);
			using type = decltype(t);
			using mem_type = typename type::member_var_type;
			//return mem_type().getPointedGObjectTypeIndex();
			return typeid(T);
		}

		void set(void* _object, GVariant& value) {
			auto t = find_type(ptr);
			using type = decltype(t);
			using mem_type = typename type::member_var_type;
			using ValueGetterType = typename std::conditional < std::is_base_of < GPropertyInterface, mem_type > ::value, GPropertyValueGetter, NonGPropertyValueGetter > ::type;
			ValueGetterType::set(_object, value, ptr, onupdate_cb);
			/*C* o = static_cast<C*>(_object);
			//(o->*ptr).set(value);// = boost::get<T>(value);
			auto t = find_type(ptr);
			using type = decltype(t);
			using mem_type = type::member_var_type;
			(o->*ptr) = GVariant::cast<mem_type&>(value);
			if (onupdate_cb) {
				onupdate_cb(o);
			}*/
		}

		GVariant get(void* _object) {
			auto t = find_type(ptr);
			using type = decltype(t);
			using mem_type = typename type::member_var_type;

			using ValueGetterType = typename std::conditional < std::is_base_of < GPropertyInterface, mem_type > ::value, GPropertyValueGetter, NonGPropertyValueGetter > ::type;
			return ValueGetterType::get(_object, ptr);
		}

		virtual GSerializer& writeBinaryValue(GSerializer& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			//return (o->*ptr).writeBinaryValue(os);
			return os;
		}

		virtual GDeserializer& readBinaryValue(GDeserializer& is, GObjectSharedPtr obj) {
			auto o = std::static_pointer_cast<C>(obj);
			//return (o->*ptr).readBinaryValue(is);
			return is;
		}

		virtual GSerializer& writeASCIIValue(GSerializer& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			//return (o->*ptr).writeASCIIValue(os);
			os << (o->*ptr);
			return os;
		}

		virtual GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj) {
			auto o = std::static_pointer_cast<C>(obj);
			//return (o->*ptr).readASCIIValue(is);
			is >> (o.get()->*ptr);
			return is;
		}

	protected:
		struct GPropertyValueGetter
		{
			static void set(void* _object, GVariant& value, T ptr, std::function<void(C*)> onupdate_cb) {
				C* o = static_cast<C*>(_object);
				auto t = find_type(ptr);
				using type = decltype(t);
				using mem_type = typename type::member_var_type;
				(o->*ptr).set(value);
				if (onupdate_cb) {
					onupdate_cb(o);
				}
			}

			static GVariant get(void* _object, T ptr)
			{
				C* o = static_cast<C*>(_object);
				auto t = find_type(ptr);
				using type = decltype(t);
				using mem_type = typename type::member_var_type;
				//mem_type v = o->*ptr;
				return ((o->*ptr).get());
			}
		};

		struct NonGPropertyValueGetter
		{
			static void set(void* _object, GVariant& value, T ptr, std::function<void(C*)> onupdate_cb) {
				C* o = static_cast<C*>(_object);
				auto t = find_type(ptr);
				using type = decltype(t);
				using mem_type = typename type::member_var_type;
				(o->*ptr) = GVariant::cast<mem_type&>(value);
				if (onupdate_cb) {
					onupdate_cb(o);
				}
			}

			static GVariant get(void* _object, T ptr)
			{
				C* o = static_cast<C*>(_object);
				return (o->*ptr);
			}
		};

	private:
		T ptr;
		std::function<void(C*)> onupdate_cb;

	};

	//for protected or private property (with getter and/or setter)
	template<typename C, typename GETTER_F, typename SETTER_F>
	class GNonPublicMetaproperty : public GMetaproperty
	{
	public:
		GNonPublicMetaproperty(const char* name, GETTER_F get, SETTER_F set, std::function<void(C*)> callback) : GMetaproperty(name), getter(get), setter(set), onupdate_cb(callback) {
		}

		std::type_index getTypeInfo() {
			return typeid(ResultType<GETTER_F>);
		}

		virtual bool isGObjectPointer() {

			using mem_type = typename std::remove_pointer<ResultType<GETTER_F> >::type;
			if (std::is_pointer<mem_type>::value)
			{
				if (std::is_base_of<GObject, typename std::remove_pointer<mem_type>::type >::value)
				{
					return true;
				}
			}
			return false;
		}

		virtual std::type_index getPointedGObjectTypeIndex() {
			using type = ResultType<GETTER_F>;
			return std::type_index(typeid(type));
		}

		void set(void* _object, GVariant& value) {
			C* o = static_cast<C*>(_object);
			//(o->*ptr).set(value);// = boost::get<T>(value);
			(o->*setter)(GVariant::cast<ArgType<SETTER_F, 1>>(value));
			if (onupdate_cb) {
				onupdate_cb(o);
			}
		}

		GVariant get(void* _object) {
			C* o = static_cast<C*>(_object);
			return (o->*getter)();
		}

		virtual GSerializer& writeBinaryValue(GSerializer& os, const GObject* obj) {
			const C* o = static_cast<const C*>(obj);
			//TODO: Write a Text/Binary deser for non GProperty
			return os;
		}

		virtual GDeserializer& readBinaryValue(GDeserializer& is, GObjectSharedPtr obj) {
			auto o = std::static_pointer_cast<C>(obj);
			//TODO: Write a Text/Binary deser for non GProperty
			return is;
		}

		virtual GSerializer& writeASCIIValue(GSerializer& os, const GObject* obj) {
			//TODO: Write a Text/Binary deser for non GProperty
			using arg_type = typename std::remove_reference<ResultType<GETTER_F> >::type;
			using ReaderType = typename std::conditional<std::is_base_of<GPropertyInterface, arg_type>::value, GPropertyHandler, NonGPropertyHandler>::type;
			return ReaderType::writeASCIIValue(os, obj, getter);
		}

		virtual GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj) {
			using arg_type = typename std::remove_reference<ArgType<SETTER_F, 1> >::type;
			using ReaderType = typename std::conditional<std::is_base_of<GPropertyInterface, arg_type>::value, GPropertyHandler, NonGPropertyHandler>::type;
			return ReaderType::readASCIIValue(is, obj, setter);
		}
	private:

		struct GPropertyHandler //the type is a GProperty
		{
			static GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj, SETTER_F setter)
			{
				using arg_type = typename std::remove_reference<ArgType<SETTER_F, 1> >::type;
				using ReaderType = typename std::conditional<std::is_base_of<GPointerPropertyInterface, arg_type>::value, ObjectPointerPropertyHandler, NonObjectPointerPropertyHandler>::type;
				return ReaderType::readASCIIValue(is, obj, setter);
			}

			static GSerializer& writeASCIIValue(GSerializer& os, const GObject* obj, GETTER_F getter) {
				const C* o = static_cast<const C*>(obj);
				using arg_type = typename std::remove_reference<ResultType<GETTER_F> >::type;
				arg_type prop((o->*getter)());
				//prop.writeASCIIValue(*os.getStream());
				os << prop;
				return os;
			}
		private:
			struct ObjectPointerPropertyHandler //the type is a GObjectpointerProperty
			{
				static GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj, SETTER_F setter)
				{
					//TODO
					auto o = std::static_pointer_cast<C>(obj);
					using arg_type = typename std::remove_reference<ArgType<SETTER_F, 1> >::type;
					arg_type prop;
					is >> (prop);
					if (prop.getObjectId())
					{
						is.addReferenceSeeker(prop.getObjectId(), obj.get(), setter);
					}
					return is;
				}
			};

			struct NonObjectPointerPropertyHandler //the type is a GProperty but not GObjectPointerProperty
			{
				static GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj, SETTER_F setter)
				{
					auto o = std::static_pointer_cast<C>(obj);
					using arg_type = typename std::remove_reference<ArgType<SETTER_F, 1> >::type;
					arg_type prop;
					is >> (prop);
					(o.get()->*setter)(prop);
					return is;
				}
			};
		};

		struct NonGPropertyHandler //the type is a NOT GProperty
		{
			static GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj, SETTER_F setter)
			{
				using arg_type = typename std::remove_reference<ArgType<SETTER_F, 1> >::type;
				using ReaderType = typename std::conditional<is_shared_ptr<arg_type>::value, SharedPointerHandler, NonPointerHandler>::type;
				return ReaderType::readASCIIValue(is, obj, setter);
			}

			static GSerializer& writeASCIIValue(GSerializer& os, const GObject* obj, GETTER_F getter) {
				const C* o = static_cast<const C*>(obj);
				using arg_type = typename std::remove_reference<ResultType<GETTER_F> >::type;
				auto prop = GPropertyUtility<typename std::remove_cv<arg_type>::type>::create((o->*getter)());
				//prop->writeASCIIValue(*os.getStream());
				os << (*prop);
				return os;
			}
		private:
			struct SharedPointerHandler //the type is some shared pointer but not shared_ptr<GObject>
			{
				static GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj, SETTER_F setter)
				{
					using arg_type = typename std::remove_reference<ArgType<SETTER_F, 1> >::type;
					using ReaderType = typename std::conditional<std::is_base_of<GObject, typename arg_type::element_type>::value, GObjectSharedPointerHandler, NonObjectSharedPointerHandler>::type;
					return ReaderType::readASCIIValue(is, obj, setter);
				}
			};

			struct GObjectSharedPointerHandler //the type is shared_ptr<GObject> but not GProperty
			{
				static GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj, SETTER_F setter)
				{
					auto prop = GPropertyUtility<GObjectSharedPtr>::create();
					is >> (*prop);
					if (prop->getObjectId())
					{
						is.addReferenceSeeker(prop->getObjectId(), obj.get(), setter);
					}
					return is;
				}
			};

			struct NonObjectSharedPointerHandler //the type is shared_ptr<T> but T is not GObject
			{
				static GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj, SETTER_F setter)
				{
					//TODO
					//static_assert(false, "Deserializing shared_ptr<T> but T is not GObject is not yet implemented");
					return is;
				}
			};

			struct NonPointerHandler
			{
				static GDeserializer& readASCIIValue(GDeserializer& is, GObjectSharedPtr obj, SETTER_F setter)
				{
					auto o = std::static_pointer_cast<C>(obj);
					using arg_type = typename std::remove_reference<ArgType<SETTER_F, 1> >::type;
					auto prop = GPropertyUtility<typename std::remove_cv<arg_type>::type>::create();
					is >> (*prop);
					(o.get()->*setter)(GVariant::cast<arg_type>(prop->get()));
					//(o.get()->*setter)(GVariant::cast<arg_type>(prop));
					return is;
				}
			};
		};
		GETTER_F getter;
		SETTER_F setter;
		std::function<void(C*)> onupdate_cb;
	};



	class GFRAMEWORK_API GMetafunction_Base
	{
	public:
		GMetafunction_Base(const char* _name) : name(_name) {
		}
		const std::string& getName() { return name; }
		virtual int invoke_lua(lua_State* L) = 0;
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
			return prototype;
		}
	protected:
		template<typename first>
		static void iterate_arg_type(lua_State* L, std::vector<GVariant>& args, int offset)
		{
			Glua_puller< typename std::decay<first>::type >(L, args, offset);
		}

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
			selection::pass(f, L, args, offset);
		}

	private:
		std::string name;
	};

	template<>
	void GMetafunction_Base::iterate_arg_type<void>(lua_State* L, std::vector<GVariant>& args, int offset);

	class GMetafunction : public GMetafunction_Base
	{
	public:
		GMetafunction(const char* _name) : GMetafunction_Base(_name) {
		}
		virtual GVariant invoke(std::vector<GVariant>& _args) = 0;
	};

	class GMetaMemberfunction : public GMetafunction_Base
	{
	public:
		GMetaMemberfunction(const char* _name) : GMetafunction_Base(_name) {
		}
		virtual GVariant invoke(void* o, std::vector<GVariant>& args) = 0;
	};

	template<typename C, typename F>
	class GMetaMemberfunction_derived : public GMetaMemberfunction
	{
	public:
		GMetaMemberfunction_derived(const char* _name, F _f) : GMetaMemberfunction(_name), func(_f) {
		}

		virtual std::string getReturntype()
		{
			return typeid(ResultType<F>).name();
		}

		virtual std::string getParameters()
		{
			return PrototypePrintHelper<F, std::make_index_sequence<Arity<F>::value> >::getPublicMemberFunctionPrototype();
		}

		virtual GVariant invoke(void* _object, std::vector<GVariant>& _args) {
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
		F func;
	};

	template<typename F>
	class GMetafunction_derived : public GMetafunction
	{
	public:
		GMetafunction_derived(const char* _name, F _f) : GMetafunction(_name), func(_f) {
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
			result = nonmember_function_invoke_helper<ResultType<F>, F, Arity<F>::value>::static_call(func, _args);
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
		F func;
	};

	class GMetaconstructor : public GMetafunction_Base
	{
	public:
		GMetaconstructor(const char* _name) : GMetafunction_Base(_name) {
		}

		template<typename T = GObject>
		T* invoke(std::vector<GVariant>& _args) {
			static_assert(std::is_base_of<GObject, T>::value, "Template argument to GMetaconstructor must be derived from GObject!");
			return static_cast<T*>(invoke_internal(_args));
		}

	protected:
		virtual GObject* invoke_internal(std::vector<GVariant>& _args) = 0;
	};

	class GFRAMEWORK_API GMetaNamespaceList
	{
	public:
		GMetaNamespaceList();
		static GMetaNamespace& _global();
		GMetaNamespace& _namespace(const char* namespace_name);
		void print(std::string indent = "");

	private:
		std::map<std::string, GMetaNamespace*> namespaceList;
	};

	class GFRAMEWORK_API GMetaNamespace
	{
	public:
		GMetaNamespace(const char* namespace_name);
		~GMetaNamespace();
		const char* getName() const;

		//GMetaclassList& getMetaclassList();
		GMetaNamespace& _namespace(const char* namespace_name);
		void print(std::string indent = "");
		GMetafunction* getMetaFunction(const char* function_name);
		GMetaNamespace* getMetaNamespace(const char* namespace_name);
		GMetaNamespace* getParentNamespace();
		template<typename FUNC>
		GMetaNamespace& function(const char* _name, FUNC _f)
		{
			auto f = new GMetafunction_derived<FUNC>(_name, _f);
			metafunctionsList.insert(std::pair<std::string, GMetafunction*>(_name, f));

			GLuaInterface::getInstance().addNamespace(getName());
			GLuaInterface::getInstance().addFunction(_name, f);

			return *this;
		}
		template<typename T>
		auto& define(const char* _name)
		{
			typedef typename std::conditional<std::is_abstract<T>::value, GMetaAbstractclass<T>, GMetaNonAbstractclass<T>>::type MetaclassType;

			MetaclassType* m = MetaclassType::getInstance();
			assert(m->name.empty());
			m->name = std::string(_name);
			metaclasslist.insert(std::make_pair(_name, m));
			m->parentNamespace = this;
			return *m;
		}

		//TODO: This could give more troubles than the benefits. Check if this can be implemented better
		/*
		template<typename T>
		static auto getMetaclassByType()
		{
			auto m = std::conditional<std::is_abstract<T>::value, GMetaAbstractclass<T>, GMetaNonAbstractclass<T>>::type::getInstance();
			return m;
		}
		*/
		GMetaclass* getMetaclass(const char* _name)
		{
			auto itr = metaclasslist.find(std::string(_name));
			if (itr == metaclasslist.end()) {
				return nullptr;
			}
			return itr->second;
		}
	private:
		std::string name;
		GMetaNamespaceList namespaceList;
		//GMetaclassList* metaclassList;
		std::map<std::string, GMetafunction*> metafunctionsList;
		std::map<std::string, GMetaclass*> metaclasslist;
		std::map<std::string, GMetaNamespace*> childMetanamespacelist;
		GMetaNamespace* parentNamespace;
	};

	template<typename C, typename F>
	class GDerivedMetaconstructor : public GMetaconstructor
	{
	public:
		GDerivedMetaconstructor(const char* _name) : GMetaconstructor(_name) {
		}

		virtual std::string getReturntype()
		{
			return std::string();
		}

		virtual std::string getPrototype()
		{
			//GMetaclass* m = GMetaNamespace::getMetaclassByType<C>();
			std::string parameters = getParameters();
			std::string prototype;
			prototype = prototype.append(" ").append(getName()).append("(");
			prototype = prototype.append(parameters);
			prototype = prototype.append(")");
			return prototype;
		}

		virtual std::string getParameters()
		{
			return PrototypePrintHelper<F, std::make_index_sequence<Arity<F>::value> >::getStaticFunctionPrototype();
		}

		GObject* invoke_internal(std::vector<GVariant>& _args) override
		{
			//result = nonmember_function_invoke_helper<ResultType<F>, C, F, Arity<F>::value>::static_call(func, _args);
			return constructor_invoke_helper<C, F, Arity<F>::value>::call(_args);
		}

		virtual int invoke_lua(lua_State* L) override
		{
			std::vector<GVariant> args;
			//TODO: Fix below
			//			fill_args_from_lua(func, L, args, 1);
			//			invoke(args);
			return 0;
		}

	private:
		//F func;
	};

	class GFRAMEWORK_API GMetaclass
	{
	public:
		const std::string& getName() { return name; }
		virtual GObjectSharedPtr createInstance() = 0;
		virtual GMetaconstructor* getConstructor(const char* _name) = 0;
		virtual GMetaMemberfunction* getPublicMemberFunction(const char* _name) = 0;
		virtual GMetaMemberfunction* getProtectedMemberFunction(const char* _name) = 0;
		virtual GMetaStaticfunction* getStaticFunction(const char* _name) = 0;
		virtual GMetaproperty* getProperty(const char* _name) = 0;
		void getConstructorsList(std::vector<std::string>& constructors_list);
		void getFunctionsList(std::vector<std::string>& functions_list);
		void getStaticFunctionsList(std::vector<std::string>& functions_list);
		void getPropertiesList(std::vector<std::string>& properties_list);
		void getEditablePropertiesList(std::vector<std::string>& properties_list);
		unsigned int getVersion();
		static GMetaclass* getBaseMetaclass(std::vector<std::string>& class_list);
		std::string getFullNamespace();
	protected:
		void addConstructor(const char* _name, GMetaconstructor* _c);
		void addPublicFunction(const char* _name, GMetaMemberfunction* _f);
		void addProtectedFunction(const char* _name, GMetaMemberfunction* _f);
		void addStaticFunction(const char* _name, GMetaStaticfunction* _f);
		void addProperty(const char* _name, GMetaproperty* _p);
		void addEditableProperty(const char* _name, GMetaproperty* _p);
		GMetaclass();

	protected:
		std::vector<std::vector<std::string>> GBaseMetaclasses;
		std::unordered_map<std::string, GMetaconstructor*> Gmetaconstructors;
		std::unordered_map<std::string, GMetaMemberfunction*> publicMemberFunctions;
		std::unordered_map<std::string, GMetaMemberfunction*> protectedMemberFunctions;
		std::unordered_map<std::string, GMetaStaticfunction*> staticMemberFunctions;
		std::unordered_map<unsigned int, std::unordered_map<std::string, GMetaproperty*>> Gmetaproperties;
		std::unordered_map<unsigned int, std::unordered_map<std::string, GMetaproperty*>> Gmetaeditableproperties;
		std::string name;
		friend GMetaNamespace;
		unsigned int properties_version;
		GMetaNamespace* parentNamespace;
	};

	template<typename T>
	class GMetaAbstractclass : public GMetaclass
	{
	public:
		static GMetaAbstractclass<T>* getInstance()
		{
			static GMetaAbstractclass<T> t;
			return &t;
		}

		GMetaAbstractclass<T>& baseMetaclass(const char* _name, std::vector<std::string>&& namespaces_list = std::vector<std::string>())
		{
			namespaces_list.push_back(_name);
			//TODO: Try to avoid duplication fo baseclass even the chances are less
			//if (std::find(GBaseMetaclasses.begin(), GBaseMetaclasses.end(), std::string(_name)) == GBaseMetaclasses.end())
			{
				GBaseMetaclasses.push_back(namespaces_list);

			}
			return *this;
		}

		//not applicable for abstract meta class. So assert if called.
		virtual GObjectSharedPtr createInstance() {
			return GObjectSharedPtr();
		}

		template<typename T2>
		GMetaAbstractclass<T2>& constructor()
		{
			return *this;
		}

		template<typename PROP>
		GMetaAbstractclass<T>& editableProperty(const char* _name, PROP _p, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GPublicMetaproperty<T, PROP>(_name, _p, callback);
			addEditableProperty(_name, p);
			return *this;
		}

		template<typename GETTER_F, typename SETTER_F>
		GMetaAbstractclass<T>& editableProperty(const char* _name, GETTER_F get, SETTER_F set, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GNonPublicMetaproperty<T, GETTER_F, SETTER_F>(_name, get, set, callback);
			addEditableProperty(_name, p);
			return *this;
		}

		template<typename PROP>
		GMetaAbstractclass<T>& property(const char* _name, PROP _p, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GPublicMetaproperty<T, PROP>(_name, _p, callback);
			addProperty(_name, p);
			return *this;
		}

		template<typename GETTER_F, typename SETTER_F>
		GMetaAbstractclass<T>& property(const char* _name, GETTER_F get, SETTER_F set, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GNonPublicMetaproperty<T, GETTER_F, SETTER_F>(_name, get, set, callback);
			addProperty(_name, p);
			return *this;
		}


		template<typename FUNC>
		GMetaAbstractclass<T>& functionPublic(const char* _name, FUNC _f)
		{
			auto f = new GMetaMemberfunction_derived<T, FUNC>(_name, _f);
			addPublicFunction(_name, f);
			return *this;
		}

		template<typename FUNC>
		GMetaAbstractclass<T>& functionProtected(const char* _name, FUNC _f)
		{
			auto f = new GMetaMemberfunction_derived<T, FUNC>(_name, _f);
			addProtectedFunction(_name, f);
			return *this;
		}

		template<typename FUNC>
		GMetaAbstractclass<T>& staticFunction(const char* _name, FUNC _f)
		{
			auto f = new GMetafunction_derived<FUNC>(_name, _f);
			addStaticFunction(_name, f);
			return *this;
		}

		GMetaAbstractclass<T>& version(unsigned int v)
		{
			properties_version = v;
			return *this;
		}

		GMetaconstructor* getConstructor(const char* _name) {
			if (Gmetaconstructors.find(std::string(_name)) != Gmetaconstructors.end())
			{
				return Gmetaconstructors[std::string(_name)];
			}
			else
			{
				return nullptr;
			}
		}

		virtual GMetaMemberfunction* getProtectedMemberFunction(const char* _name)
		{
			if (protectedMemberFunctions.find(std::string(_name)) != protectedMemberFunctions.end())
			{
				return protectedMemberFunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaMemberfunction* f = GMetaclass::getBaseMetaclass(basemetaclass)->getProtectedMemberFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return nullptr;
		}

		GMetaMemberfunction* getPublicMemberFunction(const char* _name)
		{
			if (publicMemberFunctions.find(std::string(_name)) != publicMemberFunctions.end())
			{
				return publicMemberFunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaMemberfunction* f = GMetaclass::getBaseMetaclass(basemetaclass)->getPublicMemberFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return nullptr;
		}

		GMetaStaticfunction* getStaticFunction(const char* _name)
		{
			if (staticMemberFunctions.find(std::string(_name)) != staticMemberFunctions.end())
			{
				return staticMemberFunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaStaticfunction* f = GMetaclass::getBaseMetaclass(basemetaclass)->getStaticFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return NULL;
		}

		GMetaproperty* getProperty(const char* _name)
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
				GMetaclass* mc = GMetaclass::getBaseMetaclass(basemetaclass);

				GMetaproperty* p = mc->getProperty(_name);
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
				GMetaclass* mc = GMetaclass::getBaseMetaclass(basemetaclass);
				GMetaproperty* p = mc->getProperty(_name);
				if (p != NULL)
				{
					return p;
				}
			}
			return NULL;
		}

	private:
		GMetaAbstractclass() {}
	};


	template<typename T>
	class GMetaNonAbstractclass : public GMetaclass
	{
	public:
		static GMetaNonAbstractclass<T>* getInstance()
		{
			static GMetaNonAbstractclass<T> t;
			return &t;
		}

		GMetaNonAbstractclass<T>& baseMetaclass(const char* _name, std::vector<std::string>&& namespaces_list = std::vector<std::string>())
		{
			namespaces_list.push_back(_name);
			//TODO: Try to avoid duplication fo baseclass even the chances are less
			//if (std::find(GBaseMetaclasses.begin(), GBaseMetaclasses.end(), std::string(_name)) == GBaseMetaclasses.end())
			{
				GBaseMetaclasses.push_back(namespaces_list);

			}
			return *this;
		}

		virtual GObjectSharedPtr createInstance() {
			return std::make_shared<T>();
		}

		template<typename F>
		GMetaNonAbstractclass<T>& constructor(const char* _name)
		{
			auto c = new GDerivedMetaconstructor<T, F>(_name);
			addConstructor(_name, c);
			return *this;
		}

		template<typename PROP>
		GMetaNonAbstractclass<T>& editableProperty(const char* _name, PROP _p, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GPublicMetaproperty<T, PROP>(_name, _p, callback);
			addEditableProperty(_name, p);
			return *this;
		}

		template<typename PROP>
		GMetaNonAbstractclass<T>& property(const char* _name, PROP _p, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GPublicMetaproperty<T, PROP>(_name, _p, callback);
			addProperty(_name, p);
			return *this;
		}

		template<typename GETTER_F, typename SETTER_F>
		GMetaNonAbstractclass<T>& property(const char* _name, GETTER_F get, SETTER_F set, std::function<void(T*)> callback = std::function<void(T*)>())
		{
			auto p = new GNonPublicMetaproperty<T, GETTER_F, SETTER_F>(_name, get, set, callback);
			addProperty(_name, p);
			return *this;
		}

		template<typename FUNC>
		GMetaNonAbstractclass<T>& functionPublic(const char* _name, FUNC _f)
		{
			auto f = new GMetaMemberfunction_derived<T, FUNC>(_name, _f);
			addPublicFunction(_name, f);
			return *this;
		}

		template<typename FUNC>
		GMetaNonAbstractclass<T>& functionProtected(const char* _name, FUNC _f)
		{
			auto f = new GMetaMemberfunction_derived<T, FUNC>(_name, _f);
			addProtectedFunction(_name, f);
			return *this;
		}

		template<typename FUNC>
		GMetaNonAbstractclass<T>& staticFunction(const char* _name, FUNC _f)
		{
			auto f = new GMetafunction_derived<FUNC>(_name, _f);
			addStaticFunction(_name, f);
			return *this;
		}

		GMetaNonAbstractclass<T>& version(unsigned int v)
		{
			properties_version = v;
			return *this;
		}

		GMetaconstructor* getConstructor(const char* _name) {
			auto itr = Gmetaconstructors.find(std::string(_name));
			if (itr != Gmetaconstructors.end())
			{
				return itr->second;
			}
			else
			{
				return nullptr;
			}
		}

		virtual GMetaMemberfunction* getProtectedMemberFunction(const char* _name)
		{
			if (protectedMemberFunctions.find(std::string(_name)) != protectedMemberFunctions.end())
			{
				return protectedMemberFunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaMemberfunction* f = GMetaclass::getBaseMetaclass(basemetaclass)->getProtectedMemberFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return nullptr;
		}

		GMetaMemberfunction* getPublicMemberFunction(const char* _name)
		{
			if (publicMemberFunctions.find(std::string(_name)) != publicMemberFunctions.end())
			{
				return publicMemberFunctions[std::string(_name)];
			}
			else
			{
				for (auto basemetaclass : GBaseMetaclasses)
				{
					GMetaMemberfunction* f = GMetaclass::getBaseMetaclass(basemetaclass)->getPublicMemberFunction(_name);
					if (f != NULL)
					{
						return f;
					}
				}
			}
			return NULL;
		}

		GMetaStaticfunction* getStaticFunction(const char* _name)
		{
			if (staticMemberFunctions.find(std::string(_name)) != staticMemberFunctions.end())
			{
				return staticMemberFunctions[std::string(_name)];
			}
			/*else
			{
			for (auto basemetaclass : GBaseMetaclasses)
			{
			GMetaStaticfunction* f = GMetaclass::getBaseMetaclass(basemetaclass)->getStaticFunction(_name);
			if (f != NULL)
			{
			return f;
			}
			}
			}*/
			return nullptr;
		}

		GMetaproperty* getProperty(const char* _name)
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
				GMetaproperty* p = GMetaclass::getBaseMetaclass(basemetaclass)->getProperty(_name);
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
				GMetaproperty* p = GMetaclass::getBaseMetaclass(basemetaclass)->getProperty(_name);
				if (p != NULL)
				{
					return p;
				}
			}
			return NULL;
		}

	private:
		GMetaNonAbstractclass() {}
	};


	class GFRAMEWORK_API GReflection
	{
	public:
		GReflection();
		~GReflection();
	};
}

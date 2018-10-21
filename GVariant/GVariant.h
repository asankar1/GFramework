#pragma once
#include <string>
#include <glm\vec3.hpp>
#include <boost\variant.hpp>
#include <boost\any.hpp>
#include <GFrameworkLua.h>


#ifdef VARIANT_DYNAMIC_LIBRARY
	#ifdef DLL_EXPORT
		#define LIBRARY_API __declspec( dllexport )
	#else
		#define LIBRARY_API __declspec( dllimport )
	#endif
#else
	#define LIBRARY_API
#endif

/*! \file GVariant.h
*	\brief class, functions, enums, typedefs, macros and other definitions related to GVariant class.
*/


namespace GFramework
{
	class Object;
	class Node;
	typedef std::shared_ptr<Object> ObjectSharedPtr;
	typedef std::shared_ptr<Node> NodeSharedPtr;

	int open_lua_variant_module(lua_State *L);

	template <typename... Types>
	struct myv : public boost::variant<Types...>
	{
		using boost::variant<Types...>::variant;
		template<typename T>
		operator T() const
		{
			return boost::get<T>(*this);
		}
	};

	template <typename... Types>
	struct GVariant_internal : public boost::variant<Types...>
	{
		using boost::variant<Types...>::variant;
		template<typename T>
		operator T() const
		{
			return boost::get<T>(*this);
		}
	};

	class GAny : public boost::any
	{
		GAny() : boost::any() {

		}

		template <class ValueType>
		GAny(ValueType v) : boost::any(v) {

		}
		template <class ValueType>
		GAny & operator=(ValueType&& rhs)
		{
			any(static_cast<ValueType&&>(rhs)).swap(*this);
			return *this;
		}

		template<class ValueType>
		operator ValueType() const
		{
			return boost::any_cast<ValueType>(*this);
		}

		template<class ValueType>
		operator ValueType&()
		{
			return boost::any_cast<ValueType&>(*this);
		}

		template<class ValueType>
		operator ValueType*()
		{
			return boost::any_cast<ValueType*>(*this);
		}

		template<class ValueType>
		operator const ValueType*()
		{
			return boost::any_cast<const ValueType*>(*this);
		}
	};

	class many : public boost::any
	{
	private:
		typedef struct {} unknown_type;

	public:
		many() : boost::any() { }

		many(many&& other) = default;

		void operator=(many&& rhs) {
			rhs.swap(*this);
			return;
		}

		template<typename T>
		static many create(T t) {
			typedef unknown_type type1;
			typedef typename	std::conditional<std::is_same<unknown_type, type1>::value, std::conditional< \
								std::is_lvalue_reference<T>::value, create_pointer_from_lvalue_reference<std::remove_reference<T>::type>, type1>::type, type1 >::type type2;
			typedef typename	std::conditional<std::is_same<unknown_type, type2>::value, std::conditional< \
								std::is_rvalue_reference<T>::value, create_pointer_from_rvalue_reference<std::remove_reference<T>::type>, type2 >::type, type2 >::type type3;
			typedef typename	std::conditional<std::is_same<unknown_type, type3>::value, std::conditional< \
								std::is_array<T>::value, create_as_it_is<std::decay<T>::type>, type3 >::type, type3 >::type type4;
			typedef typename	std::conditional<std::is_same<unknown_type, type4>::value, create_as_it_is<T>, type4 >::type creater_type;
			//typedef typename	std::conditional<std::is_same<unknown_type, type3>::value, create_as_it_is<T>, type3 >::type creater_type;

			//typedef typename std::conditional<std::is_reference<T>::value, create_pointer_from_reference<T>, create_as_it_is<T> >::type creater_type;
			return creater_type::create_internal(t);
		}

		template<typename T>
		static many create() = delete;

		template<>
		static many create<void>() {
			return many();
		}

		/*template<typename T>
		static T cast(many& m) {
			return boost::any_cast<T>(m);
		}*/

		template<typename T>
		static T cast(many& m)
		{
			//typedef typename std::conditional<	std::is_reference<T>::value, cast_lvalue_reference_from_pointer<T>, cast_as_it_is<T> >::type caster_type;
			typedef unknown_type type1;
			typedef typename	std::conditional<std::is_same<unknown_type, type1>::value, std::conditional< \
								std::is_lvalue_reference<T>::value, cast_lvalue_reference_from_pointer<std::remove_reference<T>::type>, type1>::type, type1 >::type type2;
			typedef typename	std::conditional<std::is_same<unknown_type, type2>::value, std::conditional< \
								std::is_rvalue_reference<T>::value, cast_rvalue_reference_from_pointer<std::remove_reference<T>::type>, type2 >::type, type2 >::type type3;
			typedef typename	std::conditional < std::is_same<unknown_type, type3>::value, std::conditional < \
								std::is_array<T>::value, cast_as_it_is <std::decay<T>::type> , type3 > ::type, type3 > ::type type4;
			typedef typename	std::conditional<std::is_same<unknown_type, type4>::value, cast_as_it_is<T>, type4 >::type caster_type;
			//typedef typename	std::conditional<std::is_same<unknown_type, type3>::value, cast_as_it_is<T>, type3 >::type caster_type;

			return caster_type::cast_internal(m);
		}

	private:
		template<typename T>
		struct create_as_it_is {
			typedef T TYPE;
			//typedef typename std::conditional<std::is_array<T>::value, typename std::decay<T>::type, T >::type TYPE;
			static many create_internal(T& t) {
				return many(static_cast<TYPE>(t));
			}
		};

		template<typename T>
		struct create_pointer_from_lvalue_reference {
			static many create_internal(T& t) {
				return many(static_cast<T*>(&t));
			}
		};

		template<typename T>
		struct create_pointer_from_rvalue_reference {
			static many create_internal(T& t) {
				return many(static_cast<T*>(&t));
			}
		};

		/*template<typename T>
		struct create_pointer_from_reference {
			typedef typename std::remove_reference<T>::type TYPE;
			static many create_internal(T& t) {
				return many(static_cast<TYPE*>(&t));
			}
		};*/

		template<typename T>
		struct cast_as_it_is {
			typedef T TYPE;
			static TYPE cast_internal(many& m) {
				return boost::any_cast<TYPE>(m);
			}
		};

		template<typename T>
		struct cast_lvalue_reference_from_pointer {
			static T& cast_internal(many& m) {
				return *boost::any_cast<T*>(m);
			}
		};

		template<typename T>
		struct cast_rvalue_reference_from_pointer {
			static T&& cast_internal(many& m) {
				return std::move(*boost::any_cast<T*>(m));
			}
		};

		template<typename T>
		many(T t) : boost::any(t) { }

#if 0
		/*template<>
		static GVariant& create<int>(int i) {
			return any(i);
		}*/

		template<typename T>
		static many ref(T& t)
		{
			auto a = any(static_cast<T*>(&t));
			many m;
			a.swap(m);
			return m;
		}

		many & operator=(many rhs)
		{
			//std::cout << "GVariant to GVariant copy" << std::endl;
			rhs.swap(*this);
			return *this;
		}

		template<typename T>
		static T cast(many& m )
		{
			return cast_helper<T>::cast(m);
		}

		/*template<typename T&>
		static T& cast(many& m)
		{
			return cast_helper<T&>::cast(m);
		}*/

		template<>
		static void cast<void>(many& m)
		{
			//return boost::any_cast<T>(m);
			return;// check_ref<std::is_reference<T>::value>::cast<std::remove_reference<T>::type>(m);
		}

		template <class T>
		many(T v) : boost::any(v) {

		}

		template <class T>
		many & operator=(T&& rhs)
		{
			//any(static_cast<T&&>(rhs)).swap(*this);
			auto a = any(static_cast<T&&>(rhs));
			a.swap(*this);
			return *this;
		}

		/*many & operator=(many& rhs)
		{
			rhs.swap(*this);
			return *this;
		}*/

		template <class T>
		many & operator=(T* rhs)
		{
			//any(static_cast<T&&>(rhs)).swap(*this);
			auto a = any(static_cast<T*>(rhs));
			a.swap(*this);
			return *this;
		}

		/*template <class T>
		many & operator=(T& rhs)
		{
			//any(static_cast<T&&>(rhs)).swap(*this);
			auto a = any(static_cast<T*>(&rhs));
			a.swap(*this);
			return *this;
		}*/

		template <class T>
		many & operator=(T& rhs)
		{
			//any(static_cast<T&&>(rhs)).swap(*this);
			auto a = any(static_cast<T>(rhs));
			a.swap(*this);
			return *this;
		}
#endif
#if 0
		template<typename T>
		operator T&()
		{
			return cast<T&>(*this);
		}

		template<typename T>
		operator const T&()
		{
			return cast<const T&>(*this);
		}

		template<typename T>
		operator T*()
		{
			return cast<T*>(*this);
		}

		template<typename T>
		operator const T*()
		{
			return cast<const T*>(*this);
		}
#endif
#if 0
		template<typename T>
		operator T&() const
		{
			//return cast<T>(*this);
			return cast_helper<T&>::cast(*this);
		}

		template<typename T>
		operator T() const
		{
			//return cast<T>(*this);
			return cast_helper<T>::cast(*this);
		}

		template<typename T>
		operator T() 
		{
			//return cast<T>(*this);
			return cast_helper<T>::cast(*this);
		}

	private:
		template<class T>
		struct cast_helper {
			static T cast(const many& m) {
				return boost::any_cast<T>(m);
			}
		};

		template<class T>
		struct cast_helper<T&> {
			static T& cast(const many& m) {
				return *(boost::any_cast<T*>(m));
			}
		};

		template<class T>
		struct cast_helper<const T&> {
			static const T& cast(const many& m) {
				return *(boost::any_cast<const T*>(m));
			}
		};
#endif
	};

	/** \var typedef boost::variant<> GVariant;
	*	\brief variant designed to handle all data in the GFramework
	*/

	/*template <typename... Types>
	struct myv : public boost::variant<Types...>
	{
		using boost::variant<Types...>::variant;
		template<typename T>
		operator T() const
		{
			return boost::get<T>(*this);
		}
		

	
	};*/



	//typedef GVariant_internal<boost::blank, bool, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, float, double, std::string, glm::vec3, ObjectSharedPtr, NodeSharedPtr> GVariant;
	//typedef myv< boost::blank, bool, char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, float, double, std::string, ObjectSharedPtr, NodeSharedPtr, double*> TVariant;
	//typedef myv<int, float, double*> GVariant;
	typedef many GVariant;
}
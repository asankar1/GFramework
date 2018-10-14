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

	struct many : public boost::any
	{
		many() : boost::any() {

		}

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
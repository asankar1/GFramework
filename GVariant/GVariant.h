#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <boost/variant.hpp>
#include <boost/any.hpp>
#include <GScript/GFrameworkLua.h>


/*! \file GVariant.h
*	\brief class, functions, enums, typedefs, macros and other definitions related to GVariant class.
*/


namespace GFramework
{
	class GObject;
	typedef std::shared_ptr<GObject> GObjectSharedPtr;

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

		many(const many& other) = default;

		template<typename T>
		many(T t) : boost::any(t) {
		}

		template<typename T>
		void operator=(T&& rhs) {
			*this = create<T>(rhs);
			return;
		}

		void operator=(many&& rhs) {
			rhs.swap(*this);
			return;
		}

		template<typename T>
		static many create(T t) {
			typedef unknown_type type1;
			typedef typename	std::conditional<std::is_same<unknown_type, type1>::value, typename std::conditional<\
								std::is_lvalue_reference<T>::value, create_pointer_from_lvalue_reference<typename std::remove_reference<T>::type>, type1>::type, type1>::type type2;
			typedef typename	std::conditional<std::is_same<unknown_type, type2>::value, typename std::conditional< \
								std::is_rvalue_reference<T>::value, create_pointer_from_rvalue_reference<typename std::remove_reference<T>::type>, type2 >::type, type2 >::type type3;
			typedef typename	std::conditional<std::is_same<unknown_type, type3>::value, typename std::conditional< \
								std::is_array<T>::value, create_as_it_is<typename std::decay<T>::type>, type3 >::type, type3 >::type type4;
			typedef typename	std::conditional<std::is_same<unknown_type, type4>::value, create_as_it_is<T>, type4 >::type creater_type;

			return creater_type::create_internal(t);
		}

		template<typename T>
		static many create() {
			static_assert(std::is_void<T>::value, "Only void type can be used to call create without arguments!");
			return many();
		}

		template<typename T>
		static T cast(many& m)
		{
			typedef unknown_type type1;
			typedef typename	std::conditional<std::is_same<unknown_type, type1>::value, typename std::conditional< \
								std::is_lvalue_reference<T>::value, cast_lvalue_reference_from_pointer<typename std::remove_reference<T>::type>, type1>::type, type1 >::type type2;
			typedef typename	std::conditional<std::is_same<unknown_type, type2>::value, typename std::conditional< \
								std::is_rvalue_reference<T>::value, cast_rvalue_reference_from_pointer<typename std::remove_reference<T>::type>, type2 >::type, type2 >::type type3;
			typedef typename	std::conditional < std::is_same<unknown_type, type3>::value, typename std::conditional < \
								std::is_array<T>::value, cast_as_it_is <typename std::decay<T>::type> , type3 > ::type, type3 > ::type type4;
			typedef typename	std::conditional<std::is_same<unknown_type, type4>::value, cast_as_it_is<T>, type4 >::type caster_type;

			return caster_type::cast_internal(m);
		}

		template<typename T>
		static T cast(many&& m)
		{
			typedef unknown_type type1;
			typedef typename	std::conditional<std::is_same<unknown_type, type1>::value, typename std::conditional< \
				std::is_lvalue_reference<T>::value, cast_lvalue_reference_from_pointer<typename std::remove_reference<T>::type>, type1>::type, type1 >::type type2;
			typedef typename	std::conditional<std::is_same<unknown_type, type2>::value, typename std::conditional< \
				std::is_rvalue_reference<T>::value, cast_rvalue_reference_from_pointer<typename std::remove_reference<T>::type>, type2 >::type, type2 >::type type3;
			typedef typename	std::conditional < std::is_same<unknown_type, type3>::value, typename std::conditional < \
				std::is_array<T>::value, cast_as_it_is <typename std::decay<T>::type>, type3 > ::type, type3 > ::type type4;
			typedef typename	std::conditional<std::is_same<unknown_type, type4>::value, cast_as_it_is<T>, type4 >::type caster_type;

			return caster_type::cast_internal(m);
		}

		template<typename T>
		static many ref(T& t)
		{
			auto a = any(static_cast<T*>(&t));
			many m;
			a.swap(m);
			return m;
		}

	private:
		template<typename T>
		struct create_as_it_is {
			typedef T TYPE;
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
	};

	typedef many GVariant;
}
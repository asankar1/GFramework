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
#include <GFramework/GScript/GLua_data_exchange.h>
#include <GFramework/GScript/GLuaScript.h>


/*! \file GReflectionHelpers.h
*	\brief Utilities and helpers for the GReflection
*/

namespace GFramework
{
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
		static std::string getPublicMemberFunctionPrototype()
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
			return static_cast<R>((casted_object->*f)(GVariant::cast<ArgType<F2, I + 1>>(args[I])...));
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
			result = static_cast<R>((casted_object->*f)());
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
			result = GVariant::ref<NON_REFERENCE_TYPE>(static_cast<R>((casted_object->*f)()));
			return result;
		}
	};

	// helper for functions taking void arguments and returning a non-void value
	template<typename R, typename C, typename F>
	struct member_function_invoke_helper<R, C, F, 1> {
		static GVariant call(void* _object, F f, std::vector<GVariant>& args)
		{
			GVariant result;
			result = member_function_invoke_helper_returns_reference < R, C, F, std::is_reference<R>::value >::call(_object, f, args);
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
	template<typename R, typename F, int n>
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
			return static_cast<R>((*f)(GVariant::cast<ArgType<F2, I>>(args[I])...));
		}
	};

	// helper for functions taking non-void arguments and returning a void value
	template<typename F, int n>
	struct nonmember_function_invoke_helper<void, F, n> {
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

	template<typename C, typename F, int n>
	struct constructor_invoke_helper {
		static C* call(std::vector<GVariant>& args)
		{
			return call_internal<F>(args, std::make_index_sequence<n>());;
		}
	private:
		template<typename F2, std::size_t... I>
		static C* call_internal(std::vector<GVariant>& args, std::index_sequence<I...>)
		{
			return new C(GVariant::cast<ArgType<F2, I>>(args[I])...);
		}
	};

	template<typename C, typename F>
	struct constructor_invoke_helper<C, F, 0> {
		static C* call(std::vector<GVariant>& args)
		{
			return new C();
		}
	};
}

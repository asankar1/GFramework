#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <limits>
#include <cmath>
#include <array>

#include <gtest/gtest.h>

#include <GFramework/GVariant/GTypes.h>
#include <GFramework/GVariant/GVariant.h>
#include <GFramework/GVariant/GObject.h>

#include "gvariant_test.h"
#ifndef Q_MOC_RUN
#include <boost/core/typeinfo.hpp>
#include <boost/variant.hpp>
#include <boost/function.hpp>
#endif

using namespace std;
using namespace GFramework;

/*	Notes:
	> using boost::get<T>() is preferred, simply using the GVariant may give issues
	> Definig a base class in the variant's templaes can accept derived class. But that base class SHOULD NOT be an abstract class; But base class pointer is acceptable.
	> if a reference is added in the variant template, then that variant can be ONLY initialized. Reassignment is not possible.
	> variants are not automatically casted to the actual type
	> cout can automatically resolve the variant type without explicit get<T>.
	> passing direct POD types to a function taking variant arguments works but automatic type cast is not as expected. For ex, 'true' is printed as 1;
*/

namespace 
{
	int int_array[3] = { 1, 2, 3 };

	enum unscoped_enum_type {
		unscoped_type1 = -1,
		unscoped_type2,
		unscoped_type3
	};

	enum class scoped_enum_type {
		scoped_type1 = -11,
		scoped_type2,
		scoped_type3
	};

	struct RGB {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	};

	union Color {
		unsigned char channel[3];
		RGB rgb;
	};

	int add(int a, int b) {
		return a + b;
	}

	int sub(int a, int b) {
		return a - b;
	}

	class simple_class {

	};

	class abstract_class {
	public:
		virtual void initialize() = 0;
	};

	class interface_class {
	protected:
		interface_class() {}
	};

	class non_copyable_class {
	public:
		non_copyable_class() {}
		non_copyable_class(const non_copyable_class&) = delete;
		non_copyable_class& operator=(const non_copyable_class&) = delete;
		int get_number()const {
			return number;
		}
		void set_number(int n) {
			number = n;
		}
	private:
		int number = 14;
	};

	class non_moveable_class {
	public:
		non_moveable_class() {}
		non_moveable_class(non_moveable_class&&) = delete;
		int get_number() const {
			return number;
		}
		void set_number(int n) {
			number = n;
		}
	private:
		int number = 27;
	};

	class base_class {
	public:
		static float get_static_PI() {
			return 3.14f;
		}

		virtual int get_virtual_id() {
			return 11;
		}

		int base_only_func() {
			return 33;
		}

		int common_non_virtual_func() {
			return 55;
		}
	};

	class derived_class : public base_class {
	public:
		virtual int get_virtual_id() override {
			return 22;
		}

		int derive_only_func() {
			return 44;
		}

		int common_non_virtual_func() {
			return 66;
		}

		int lref_qualified_member_func()& {
			return 77;
		}

		int rref_qualified_member_func()&& {
			return 88;
		}
	};

	void process_base_class(base_class* bc) {

	}

	typedef int(*non_member_func_ptr(float));

	typedef int(base_class::* virtual_member_func_ptr(void));

	typedef int(derived_class::* nonvirtual_member_func_ptr(void));
}

#define MAX_TESTCASE_NAME_LENGTH  100

template<typename T>
typename std::enable_if<!std::is_floating_point<T>::value, bool>::type 
test_gvariant_special_float_values(GVariant& gv)
{
	return true;
}

template<typename T, typename U=T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
test_gvariant_special_float_values(GVariant& gv)
{
	bool result = true;
	if (numeric_limits<U>::has_infinity)
	{
		gv = GVariant::create<T>(numeric_limits<U>::infinity());
		result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::infinity());
	}

	if (numeric_limits<U>::has_quiet_NaN)
	{
		gv = GVariant::create<T>(numeric_limits<U>::quiet_NaN());
		result = result && (std::isnan(GVariant::cast<T>(gv))); //NaN is always != Nan, use std::isnan
	}

	if (numeric_limits<U>::has_signaling_NaN)
	{
		gv = GVariant::create<T>(numeric_limits<U>::signaling_NaN());
		result = result && (std::isnan(GVariant::cast<T>(gv))); //NaN is always != Nan, use std::isnan
	}

	if (numeric_limits<U>::has_denorm)
	{
		gv = GVariant::create<T>(numeric_limits<U>::denorm_min());
		result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::denorm_min());
	}
	return result;
}

template<typename T>
bool test_gvariant_fundemental_types(GVariant& gv)
{
	bool result = true;

	//constexpr range
	gv = GVariant::create<T>(numeric_limits<T>::min());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<T>::min());
	gv = GVariant::create<T>(numeric_limits<T>::lowest());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<T>::lowest());
	gv = GVariant::create<T>(numeric_limits<T>::max());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<T>::max());

	result = result && test_gvariant_special_float_values<T>(gv);

	//lvalue
	T v1 = numeric_limits<T>::min();
	gv = GVariant::create<T>(v1);
	result = result && (GVariant::cast<T>(gv) == v1);

	//lvalue reference
	T& v2 = v1;
	gv = GVariant::create<T>(v2);
	result = result && (GVariant::cast<T>(gv) == v2);

	//const lvalue
	const T v3 = numeric_limits<T>::min();
	gv = GVariant::create<T>(v3);
	result = result && (GVariant::cast<T>(gv) == v3);

	//const lvalue reference
	const T& v4 = v1;
	gv = GVariant::create<T>(v4);
	result = result && (GVariant::cast<T>(gv) == v4);

	//rvalue
	auto ret_rvalue = []()->T {T v = numeric_limits<T>::min(); return v; };
	gv = GVariant::create<T>(ret_rvalue());
	result = result && (GVariant::cast<T>(gv) == ret_rvalue());

	//rvalue reference
	T&& v5 = numeric_limits<T>::min();
	gv = GVariant::create<T>(v5);
	result = result && (GVariant::cast<T>(gv) == v5);

	//raw pointer
	T* v6 = &v1;
	gv = GVariant::create<T*>(v6);
	result = result && (GVariant::cast<T*>(gv) == v6);
	result = result && (*GVariant::cast<T*>(gv) == v1);
	return result;
}

template<typename T, typename U>
bool test_gvariant_gproperty_types(GVariant& gv)
{
	bool result = true;

	//constexpr range
	gv = GVariant::create<T>(numeric_limits<U>::min());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::min());
	gv = GVariant::create<T>(numeric_limits<U>::lowest());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::lowest());
	gv = GVariant::create<T>(numeric_limits<U>::max());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::max());

	result = result && test_gvariant_special_float_values<T>(gv);

	//lvalue
	T v1 = numeric_limits<U>::min();
	gv = GVariant::create<T>(v1);
	result = result && (GVariant::cast<T>(gv) == v1);

	//lvalue reference
	T& v2 = v1;
	gv = GVariant::create<T>(v2);
	result = result && (GVariant::cast<T>(gv) == v2);

	//const lvalue
	const T v3 = numeric_limits<U>::min();
	gv = GVariant::create<T>(v3);
	result = result && (GVariant::cast<T>(gv) == v3);

	//const lvalue reference
	const T& v4 = v1;
	gv = GVariant::create<T>(v4);
	result = result && (GVariant::cast<T>(gv) == v4);

	//rvalue
	auto ret_rvalue = []()->T {T v = numeric_limits<U>::min(); return v; };
	gv = GVariant::create<T>(ret_rvalue());
	result = result && (GVariant::cast<T>(gv) == ret_rvalue());

	//rvalue reference
	T&& v5 = numeric_limits<U>::min();
	gv = GVariant::create<T>(v5);
	result = result && (GVariant::cast<T>(gv) == v5);

	//raw pointer
	T* v6 = &v1;
	gv = GVariant::create<T*>(v6);
	result = result && (GVariant::cast<T*>(gv) == v6);
	result = result && (*GVariant::cast<T*>(gv) == v1);
	return result;
}

template<>
bool test_gvariant_fundemental_types<void>(GVariant& gv)
{
	bool result = true;
	gv = GVariant::create<void>(); //test if variant is able to hold void
	return result;
}

#define PRINT_TESTCASE_NAME(STR) (cout << std::left << std::setw(MAX_TESTCASE_NAME_LENGTH) << "Testing GVariant for type '" STR "': ")
#define PRINT_RESULT(STR) (cout << "[" << std::right << std::setw(14) << STR "]" << endl)
#define TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE( TYPE ) \
		{\
			PRINT_TESTCASE_NAME(#TYPE); \
			if (test_gvariant_fundemental_types<TYPE>(gv)) \
				PRINT_RESULT("SUCCESS"); \
			else \
				PRINT_RESULT("FAILED"); \
		}

#define TEST_GVARIANT_FOR_GPROPERTY_TYPE( PROP_TYPE ) \
		{\
			PRINT_TESTCASE_NAME(#PROP_TYPE ); \
			if (test_gvariant_gproperty_types<PROP_TYPE , PROP_TYPE ::type>(gv)) \
				PRINT_RESULT("SUCCESS"); \
			else \
				PRINT_RESULT("FAILED"); \
		}

bool test_gvariant_compund_types_raw_pointer(GVariant& gv)
{
	bool result = true;
	float f1 = 1.0f;
	float* f2 = &f1;
	const float* f3 = &f1;
	float* const f4 = &f1;

	gv = GVariant::create(&f1);
	result = result && (GVariant::cast<float*>(gv) == &f1);
	result = result && (*GVariant::cast<float*>(gv) == f1);

	gv = GVariant::create(f2);
	result = result && (GVariant::cast<float*>(gv) == &f1);
	result = result && (*GVariant::cast<float*>(gv) == f1);

	gv = GVariant::create(f3);
	result = result && (GVariant::cast<const float*>(gv) == &f1);
	result = result && (*GVariant::cast<const float*>(gv) == f1);

	gv = GVariant::create(f4);
	result = result && (GVariant::cast<float*>(gv) == &f1);
	result = result && (*GVariant::cast<float* const>(gv) == f1);

	return result;
}

bool test_gvariant_compund_types_one_dim_array(GVariant& gv)
{
	bool result = true;

	int arr1[] = { 1,2,3,4,5 };
	gv = GVariant::create(arr1);
	result = result && (GVariant::cast<int*>(gv)[0] == arr1[0]);
	result = result && (GVariant::cast<int*>(gv)[4] == arr1[4]);

	return result;
}

bool test_gvariant_compund_types_multi_dim_array(GVariant& gv)
{
	bool result = false;
	//TODO:bad exceptioption thrown for multi-dimension array
	/*int arr2[2][5] = { { 1,2,3,4,5 },{ 1,2,3,4,5 } };
	gv = GVariant::create(arr2);
	result = result && (GVariant::cast<int*>(gv)[0] == arr2[0][0]);
	result = result && (GVariant::cast<int*>(gv)[1] == arr2[1][4]);*/
	return result;
}

bool test_gvariant_compund_types_shared_ptr(GVariant& gv)
{
	bool result = false;

	//shared pointer
	auto v7 = std::make_shared<int>();
	gv = GVariant::create<std::shared_ptr<int>>(v7);
	result = result && (GVariant::cast<std::shared_ptr<int>>(gv) == v7);

	//unique pointer
	//TODO: boost::any does not support Not Copyable types, see if this can be fixed
#if 0
	auto v8 = std::make_unique<T>();
	gv = GVariant::create<std::unique_ptr<T>>(std::move(v8));
	result = result && (GVariant::cast<std::unique_ptr<T>>(gv) == v8);
#endif

	return result;
}

bool test_gvariant_compund_types_unique_ptr(GVariant& gv)
{
	bool result = false;

	//unique pointer
	//TODO: boost::any does not support Not Copyable types, see if this can be fixed
#if 0
	auto v8 = std::make_unique<T>();
	gv = GVariant::create<std::unique_ptr<T>>(std::move(v8));
	result = result && (GVariant::cast<std::unique_ptr<T>>(gv) == v8);
#endif
	return result;
}

bool test_gvariant_compund_types_lvalue_ref(GVariant& gv)
{
	bool result = false;
#if 0
	int a1 = 123;
	int& a2 = a1;

	gv = GVariant::create(a2);
	a1 = 456;
	result = result && (GVariant::cast<int&>(gv) == a1);

	const int& a3 = a1;
	gv = GVariant::create(a3);
	a1 = 789;
	result = result && (GVariant::cast<int&>(gv) == a1);
#endif
	return result;
}

bool test_gvariant_compund_types_rvalue_ref(GVariant& gv)
{
	bool result = true;
	int&& a1 = 123;
	auto a2 = std::move(a1);

	//TODO: check why casting to int&& fails
	gv = GVariant::create(a1);
	result = result && (GVariant::cast<int>(gv) == a1);

	gv = GVariant::create(a2);
	result = result && (GVariant::cast<int>(gv) == a2);

	return result;
}

bool test_gvariant_compund_types_member_variable_pointer(GVariant& gv)
{
	bool result = true;
	RGB brown = { 255, 128, 0 };
	RGB* brown_ptr = &brown;
	const RGB* const_brown_ptr = &brown;

	unsigned char  RGB::* red_ptr = &RGB::red;
	brown.*red_ptr = 20;
	gv = GVariant::create<unsigned char RGB::*>(red_ptr);
	result = result && (brown.*GVariant::cast<unsigned char RGB::*>(gv) == 20);
	return result;
}

bool test_gvariant_compund_types_member_function_pointer(GVariant& gv)
{
	bool result = true;

	base_class bc;
	derived_class dc;
	base_class& dbc = dc;

	//general member function
	typedef int (derived_class::* general_func_ptr)(void);
	general_func_ptr derive_only_func_ptr = &derived_class::derive_only_func;
	gv = GVariant::create<general_func_ptr>(derive_only_func_ptr);
	result = result && ((dc.*GVariant::cast<general_func_ptr>(gv))() == 44);

	//lvalue ref qualified member function
	typedef int (derived_class::* lvalue_ref_qualified_func_ptr)(void)&;
	lvalue_ref_qualified_func_ptr lref_qualified_member_func_ptr = &derived_class::lref_qualified_member_func;
	gv = GVariant::create<lvalue_ref_qualified_func_ptr>(lref_qualified_member_func_ptr);
	result = result && ((dc.*GVariant::cast<lvalue_ref_qualified_func_ptr>(gv))() == 77);

	//rvalue ref qualified member function
	typedef int (derived_class::* rvalue_ref_qualified_func_ptr)(void)&&;
	rvalue_ref_qualified_func_ptr rref_qualified_member_func_ptr = &derived_class::rref_qualified_member_func;
	gv = GVariant::create<rvalue_ref_qualified_func_ptr>(rref_qualified_member_func_ptr);
	result = result && ((std::move(dc).*GVariant::cast<rvalue_ref_qualified_func_ptr>(gv))() == 88);

	//virtual member function
	typedef int (derived_class::* derived_virtual_func_ptr)(void);
	typedef int (base_class::* base_virtual_func_ptr)(void);
	derived_virtual_func_ptr derived_virtual_func = &derived_class::get_virtual_id;
	base_virtual_func_ptr base_virtual_func = &base_class::get_virtual_id;

	gv = GVariant::create<base_virtual_func_ptr>(base_virtual_func);
	result = result && ((bc.*GVariant::cast<base_virtual_func_ptr>(gv))() == 11);

	gv = GVariant::create<derived_virtual_func_ptr>(derived_virtual_func);
	result = result && ((dc.*GVariant::cast<derived_virtual_func_ptr>(gv))() == 22);

	gv = GVariant::create<base_virtual_func_ptr>(base_virtual_func);
	result = result && ((dbc.*GVariant::cast<base_virtual_func_ptr>(gv))() == 22);

	gv = GVariant::create<base_virtual_func_ptr>(base_virtual_func);
	result = result && ((dc.*GVariant::cast<base_virtual_func_ptr>(gv))() == 22);

	//non virtual function
	typedef int (derived_class::* common_non_virtual_func_ptr)(void);
	common_non_virtual_func_ptr common_non_virtual_func = &derived_class::common_non_virtual_func;
	gv = GVariant::create<common_non_virtual_func_ptr>(common_non_virtual_func);
	result = result && ((dc.*GVariant::cast<common_non_virtual_func_ptr>(gv))() == 66);

	return result;
}

bool test_gvariant_compund_types_class(GVariant& gv)
{
	bool result = true;

	non_copyable_class ncc;
	non_moveable_class nmc;

	//gv = GVariant::create<non_copyable_class>(ncc); //expected compile error

	gv = GVariant::create<non_copyable_class&>(ncc);
	ncc.set_number(17);
	result = result && (GVariant::cast<non_copyable_class&>(gv).get_number() == 17);

	gv = GVariant::create<const non_copyable_class&>(ncc);
	ncc.set_number(22);
	result = result && (GVariant::cast<const non_copyable_class&>(gv).get_number() == 22);

	gv = GVariant::create<non_copyable_class&&>(std::move(ncc));
	ncc.set_number(17);
	result = result && (GVariant::cast<non_copyable_class&&>(gv).get_number() == 17);

	gv = GVariant::create<non_copyable_class&&>(non_copyable_class());
	result = result && (GVariant::cast<non_copyable_class&&>(gv).get_number() == 14);

	gv = GVariant::create<const non_copyable_class&&>(std::move(ncc));
	ncc.set_number(17);
	result = result && (GVariant::cast<const non_copyable_class&&>(gv).get_number() == 17);

	gv = GVariant::create<const non_copyable_class&&>(non_copyable_class());
	result = result && (GVariant::cast<const non_copyable_class&&>(gv).get_number() == 14);

	gv = GVariant::create<non_copyable_class*>(&ncc);
	ncc.set_number(37);
	result = result && (GVariant::cast<non_copyable_class*>(gv)->get_number() == 37);

	gv = GVariant::create<const non_copyable_class*>(&ncc);
	ncc.set_number(42);
	result = result && (GVariant::cast<const non_copyable_class*>(gv)->get_number() == 42);

	//gv = GVariant::create<non_moveable_class>(nmc); //expected compile error

	gv = GVariant::create<non_moveable_class&>(nmc);
	nmc.set_number(17);
	result = result && (GVariant::cast<non_moveable_class&>(gv).get_number() == 17);

	gv = GVariant::create<const non_moveable_class&>(nmc);
	nmc.set_number(22);
	result = result && (GVariant::cast<const non_moveable_class&>(gv).get_number() == 22);

	gv = GVariant::create<non_moveable_class*>(&nmc);
	nmc.set_number(37);
	result = result && (GVariant::cast<non_moveable_class*>(gv)->get_number() == 37);

	gv = GVariant::create<const non_moveable_class*>(&nmc);
	nmc.set_number(42);
	result = result && (GVariant::cast<const non_moveable_class*>(gv)->get_number() == 42);

	return result;
}

bool test_gvariant_compund_types_function(GVariant& gv)
{
	bool result = true;
	auto f1 = []()->int {return 345; };
	gv = GVariant::create(f1);
	result = result && (GVariant::cast<decltype(f1)>(gv)() == 345);

	return result;
}

bool test_gvariant_compund_types_enum(GVariant& gv)
{
	bool result = true;
	enum BgColor
	{
		black,
		white,
		blue
	};
	
	BgColor v1 = BgColor::black;
	gv = GVariant::create(v1);
	result = result && (GVariant::cast<BgColor >(gv) == BgColor::black);
	return result;
}

bool test_gvariant_compund_types_union(GVariant& gv)
{
	bool result = true;
	union id
	{
		uint8 sender_id;
		uint8 receiver_id;
	};

	id v1;
	v1.sender_id = 243;

	gv = GVariant::create(v1);
	result = result && (GVariant::cast<id>(gv).receiver_id == v1.sender_id);
	return result;
}

bool test_gvariant_compund_types_GVariant(GVariant& gv)
{
	bool result = true;

	int i = 4;
	GVariant gv2 = i;

	gv = (gv2);
	result = result && (GVariant::cast<int>(gv2) == 4);

	gv = GVariant::create(gv2);
	result = result && (GVariant::cast<int>(gv2) == 4);

	return result;
}

bool test_gvariant_compund_types_string(GVariant& gv)
{
	bool result = true;

	std::string  v1 = "Test str";
	gv = GVariant::create(v1);
	result = result && (GVariant::cast<std::string>(gv) == v1);
	return result;
}

bool test_gvariant_compund_types_wstring(GVariant& gv)
{
	bool result = true;

	std::wstring  v1 = L"Test str";
	gv = GVariant::create(v1);
	result = result && (GVariant::cast<std::wstring>(gv) == v1);

	return result;
}

bool test_gvariant_compund_types_vector(GVariant& gv)
{
	bool result = true;

	std::vector<uint64> objectid;
	objectid.push_back(123);
	objectid.push_back(456);
	objectid.push_back(789);
	gv = GVariant::create(objectid);
	result = result && (GVariant::cast<std::vector<uint64>>(gv)[0] ==123);
	result = result && (GVariant::cast<std::vector<uint64>>(gv)[1] ==456);
	result = result && (GVariant::cast<std::vector<uint64>>(gv)[2] ==789);
	return result;
}

bool test_gvariant_compund_types_list(GVariant& gv)
{
	bool result = true;

	std::list<uint64> objectid;
	objectid.push_back(123);
	objectid.push_back(456);
	objectid.push_back(789);

	gv = GVariant::create(objectid);

	//Must store the casted value into a variable,
	//calling (GVariant::cast<decltype(objectid)>(gv)).begin() will leave a dangling reference
	auto v = (GVariant::cast<decltype(objectid)>(gv));
	auto itr = v.begin();
	result = result && (*itr == 123);
	itr++;
	result = result && (*itr == 456);
	itr++;
	result = result && (*itr == 789);
	itr++;
	result = result && (itr == v.end());
	return result;
}

bool test_gvariant_compund_types_map(GVariant& gv)
{
	bool result = true;
	std::map < uint64, glm::vec4> boundingrects;
	boundingrects[123] = glm::vec4(1);
	boundingrects[456] = glm::vec4(2);
	boundingrects[789] = glm::vec4(3);
	gv = GVariant::create(boundingrects);
	result = result && (GVariant::cast<std::map < uint64, glm::vec4>>(gv)[123] == glm::vec4(1));
	result = result && (GVariant::cast<std::map < uint64, glm::vec4>>(gv)[456] == glm::vec4(2));
	result = result && (GVariant::cast<std::map < uint64, glm::vec4>>(gv)[789] == glm::vec4(3));
	return result;
}

bool test_gvariant_compund_types_tuple(GVariant& gv)
{
	bool result = true;

	std::tuple< float32, glm::vec2, std::string> circle;
	circle = std::make_tuple<float32, glm::vec2, std::string>(3.14f, glm::vec2(123,456), "Circle1");
	gv = GVariant::create(circle);
	
	result = result && (std::get<0>(GVariant::cast<decltype(circle)>(gv)) == 3.14f);
	result = result && (std::get<1>(GVariant::cast<decltype(circle)>(gv)) == glm::vec2(123, 456));
	result = result && (std::get<2>(GVariant::cast<decltype(circle)>(gv)) == "Circle1");

	return result;
}


#if 1
void run_variant_testcases()
{
	cout << "==========================" << endl;
	cout << "Startig GVariant testcases" << endl;
	cout << "==========================" << endl;
	cout << "                          " << endl;

	//its must to keep the gv i nthe outer scope to ensure if 
	//can be reassigned with differnt type values
	GVariant gv;

	{
		cout << "Testing GVariant for fundemental types " << endl;
		cout << "-------------------------------------- " << endl;

		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(void);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(std::nullptr_t);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(bool);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(int8);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(uint8);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(char);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(char16_t);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(char32_t);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(int16);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(uint16);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(int32);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(uint32);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(int64);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(uint64);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(float32);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(float64);
		TEST_GVARIANT_FOR_FUNDEMENTAL_TYPE(float80);
	}

	{
		cout << "                                    " << endl;
		cout << "Testing GVariant for compound types " << endl;
		cout << "----------------------------------- " << endl;
		test_gvariant_compund_types_raw_pointer(gv);
		test_gvariant_compund_types_one_dim_array(gv);
		test_gvariant_compund_types_multi_dim_array(gv);
		test_gvariant_compund_types_shared_ptr(gv);
		test_gvariant_compund_types_unique_ptr(gv);
		test_gvariant_compund_types_lvalue_ref(gv);
		test_gvariant_compund_types_rvalue_ref(gv);
		test_gvariant_compund_types_member_variable_pointer(gv);
		test_gvariant_compund_types_member_function_pointer(gv);
		test_gvariant_compund_types_class(gv);
		test_gvariant_compund_types_function(gv);
		test_gvariant_compund_types_enum(gv);
		test_gvariant_compund_types_union(gv);
		test_gvariant_compund_types_GVariant(gv);
		test_gvariant_compund_types_string(gv);
		test_gvariant_compund_types_wstring(gv);
		test_gvariant_compund_types_vector(gv);
		test_gvariant_compund_types_list(gv);
		test_gvariant_compund_types_map(gv);
		test_gvariant_compund_types_tuple(gv);

	}

	{
		cout << "                                              " << endl;
		cout << "Testing GVariant for built-in GProperty types " << endl;
		cout << "--------------------------------------------- " << endl;
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GBoolProperty);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GCharProperty);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GInt8Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GUint8Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GInt16Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GUint16Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GInt32Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GUint32Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GInt64Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GUint64Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GFloatProperty);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GDoubleProperty);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GVec2Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GVec3Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GVec4Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GMat2Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GMat3Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GMat4Property);
		TEST_GVARIANT_FOR_GPROPERTY_TYPE(GObjectPointerProperty);
	}

}

#else
void GFRAMEWORK_TEST_API run_variant_testcases()
{
	cout << "==========================" << endl;
	cout << "Startig GVariant testcases" << endl;
	cout << "==========================" << endl;

	//fundamental types
	{
		cout << "Testing GVariant for fundemental types: ";
		GVariant gv;

		gv = 2;
		//char c = (int)gv;
		char c = GVariant::cast<int>(gv);
		gv = (int)3;
		GVariant gv2(gv);
		vector<GVariant> args;
		args.push_back(gv2);
		GVariant gv3(3U);
		args.push_back((int)5);

		{
			int i = 23;
			vector<GVariant> args1;
			const int& re = i;
			GVariant g;
			g = GVariant::create<const int&>(re);
			args1.push_back(g);
			const int& tt = GVariant::cast<const int&>(g);
			const int& tt2 = (const int&)g;
		}

		//explicit type specification
		gv = GVariant::create<void>();

		//gv = GVariant::create<int>();

		gv = GVariant::create<nullptr_t>(nullptr);
		assert(GVariant::cast<nullptr_t>(gv) == nullptr);

		gv = GVariant::create<bool>(numeric_limits<bool>::max());
		assert(GVariant::cast<bool>(gv) == numeric_limits<bool>::max());

		gv = GVariant::create<char>(numeric_limits<char>::max());
		assert(GVariant::cast<char>(gv) == numeric_limits<char>::max());

		gv = GVariant::create<signed char>(numeric_limits<signed char>::min());
		assert(GVariant::cast<signed char>(gv) == numeric_limits<signed char>::min());

		gv = GVariant::create<unsigned char>(numeric_limits<unsigned char>::max());
		assert(GVariant::cast<unsigned char>(gv) == numeric_limits<unsigned char>::max());

		gv = GVariant::create<wchar_t>(numeric_limits<wchar_t>::max());
		assert(GVariant::cast<wchar_t>(gv) == numeric_limits<wchar_t>::max());

		gv = GVariant::create<char16_t>(numeric_limits<char16_t>::min());
		assert(GVariant::cast<char16_t>(gv) == numeric_limits<char16_t>::min());

		gv = GVariant::create<char32_t>(numeric_limits<char32_t>::min());
		assert(GVariant::cast<char32_t>(gv) == numeric_limits<char32_t>::min());

		gv = GVariant::create<short>(numeric_limits<short>::min());
		assert(GVariant::cast<short>(gv) == numeric_limits<short>::min());

		gv = GVariant::create<unsigned short>(numeric_limits<unsigned short>::max());
		assert(GVariant::cast<unsigned short>(gv) == numeric_limits<unsigned short>::max());

		gv = GVariant::create<int>(numeric_limits<int>::min());
		assert(GVariant::cast<int>(gv) == numeric_limits<int>::min());

		gv = GVariant::create<unsigned int>(numeric_limits<unsigned int>::max());
		assert(GVariant::cast<unsigned int>(gv) == numeric_limits<unsigned int>::max());

		gv = GVariant::create<long>(numeric_limits<long>::min());
		assert(GVariant::cast<long>(gv) == numeric_limits<long>::min());

		gv = GVariant::create<unsigned long>(numeric_limits<unsigned long>::max());
		assert(GVariant::cast<unsigned long>(gv) == numeric_limits<unsigned long>::max());

		gv = GVariant::create<long long>(numeric_limits<long long>::min());
		assert(GVariant::cast<long long>(gv) == numeric_limits<long long>::min());

		gv = GVariant::create<unsigned long long>(numeric_limits<unsigned long long>::max());
		assert(GVariant::cast<unsigned long long>(gv) == numeric_limits<unsigned long long>::max());

		gv = GVariant::create<unscoped_enum_type>(unscoped_type2);
		assert(GVariant::cast<unscoped_enum_type>(gv) == unscoped_type2);

		gv = GVariant::create<scoped_enum_type>(scoped_enum_type::scoped_type3);
		assert(GVariant::cast<scoped_enum_type>(gv) == scoped_enum_type::scoped_type3);

		gv = GVariant::create<float>(numeric_limits<float>::max());
		assert(GVariant::cast<float>(gv) == numeric_limits<float>::max());

		gv = GVariant::create<float>(numeric_limits<float>::quiet_NaN());
		assert(isnan(GVariant::cast<float>(gv)));

		gv = GVariant::create<float>(numeric_limits<float>::signaling_NaN());
		assert(isnan(GVariant::cast<float>(gv)));

		gv = GVariant::create<float>(numeric_limits<float>::infinity());
		assert(isinf(GVariant::cast<float>(gv)));

		gv = GVariant::create<double>(numeric_limits<double>::max());
		assert(GVariant::cast<double>(gv) == numeric_limits<double>::max());

		gv = GVariant::create<double>(numeric_limits<double>::quiet_NaN());
		assert(isnan(GVariant::cast<double>(gv)));

		gv = GVariant::create<double>(numeric_limits<double>::signaling_NaN());
		assert(isnan(GVariant::cast<double>(gv)));

		gv = GVariant::create<double>(numeric_limits<double>::infinity());
		assert(isinf(GVariant::cast<double>(gv)));

		gv = GVariant::create<long double>(numeric_limits<long double>::max());
		assert(GVariant::cast<long double>(gv) == numeric_limits<long double>::max());

		gv = GVariant::create<long double>(numeric_limits<long double>::quiet_NaN());
		assert(isnan(GVariant::cast<long double>(gv)));

		gv = GVariant::create<long double>(numeric_limits<long double>::signaling_NaN());
		assert(isnan(GVariant::cast<long double>(gv)));

		gv = GVariant::create<long double>(numeric_limits<long double>::infinity());
		assert(isinf(GVariant::cast<long double>(gv)));

		gv = GVariant::create<std::string>("TestStr");
		assert(GVariant::cast<std::string>(gv) == std::string("TestStr"));

		std::string another_str = "AnotherStr";
		gv = GVariant::create<std::string>(another_str);
		another_str = "modified";
		assert(GVariant::cast<std::string>(gv) == std::string("AnotherStr"));
		cout << "Ok" << endl;
	}

	//compound type - reference(lvalue and rvalue)
	{
#define CHECK_LVALUE_REFERENCE_TYPE(TYPE) {	TYPE val = numeric_limits<TYPE>::max(); gv = GVariant::create<TYPE&>(val); \
													val = numeric_limits<TYPE>::min(); assert(GVariant::cast<TYPE&>(gv) == numeric_limits<TYPE>::min()); }
#define CHECK_CONST_LVALUE_REFERENCE_TYPE(TYPE) {	TYPE val = numeric_limits<TYPE>::max(); gv = GVariant::create<const TYPE&>(val); \
													val = numeric_limits<TYPE>::min(); assert(GVariant::cast<const TYPE&>(gv) == numeric_limits<TYPE>::min()); }

#define CHECK_RVALUE_REFERENCE_TYPE(TYPE) {	TYPE val = numeric_limits<TYPE>::max(); gv = GVariant::create<TYPE&&>(std::move(val)); \
													assert(GVariant::cast<TYPE&&>(gv) == numeric_limits<TYPE>::max()); }
#define CHECK_CONST_RVALUE_REFERENCE_TYPE(TYPE) {	TYPE val = numeric_limits<TYPE>::max(); gv = GVariant::create<const TYPE&&>(std::move(val)); \
													assert(GVariant::cast<const TYPE&&>(gv) == numeric_limits<TYPE>::max()); }

		cout << "Testing GVariant for fundemental type references: ";
		GVariant gv;

		CHECK_LVALUE_REFERENCE_TYPE(bool);
		CHECK_LVALUE_REFERENCE_TYPE(char);
		CHECK_LVALUE_REFERENCE_TYPE(signed char);
		CHECK_LVALUE_REFERENCE_TYPE(unsigned char);
		CHECK_LVALUE_REFERENCE_TYPE(wchar_t);
		CHECK_LVALUE_REFERENCE_TYPE(char16_t);
		CHECK_LVALUE_REFERENCE_TYPE(char32_t);
		CHECK_LVALUE_REFERENCE_TYPE(short);
		CHECK_LVALUE_REFERENCE_TYPE(unsigned short);
		CHECK_LVALUE_REFERENCE_TYPE(int);
		CHECK_LVALUE_REFERENCE_TYPE(unsigned int);
		CHECK_LVALUE_REFERENCE_TYPE(long);
		CHECK_LVALUE_REFERENCE_TYPE(unsigned long);
		CHECK_LVALUE_REFERENCE_TYPE(long long);
		CHECK_LVALUE_REFERENCE_TYPE(unsigned long long);
		CHECK_LVALUE_REFERENCE_TYPE(float);
		CHECK_LVALUE_REFERENCE_TYPE(float);
		CHECK_LVALUE_REFERENCE_TYPE(float);
		CHECK_LVALUE_REFERENCE_TYPE(float);
		CHECK_LVALUE_REFERENCE_TYPE(double);
		CHECK_LVALUE_REFERENCE_TYPE(double);
		CHECK_LVALUE_REFERENCE_TYPE(double);
		CHECK_LVALUE_REFERENCE_TYPE(double);
		CHECK_LVALUE_REFERENCE_TYPE(long double);
		CHECK_LVALUE_REFERENCE_TYPE(long double);
		CHECK_LVALUE_REFERENCE_TYPE(long double);
		CHECK_LVALUE_REFERENCE_TYPE(long double);
		std::string str = "TestStr";
		gv = GVariant::create<std::string&>(str);
		str = "modified";
		assert(GVariant::cast<std::string&>(gv) == std::string("modified"));

		CHECK_RVALUE_REFERENCE_TYPE(bool);
		CHECK_RVALUE_REFERENCE_TYPE(char);
		CHECK_RVALUE_REFERENCE_TYPE(signed char);
		CHECK_RVALUE_REFERENCE_TYPE(unsigned char);
		CHECK_RVALUE_REFERENCE_TYPE(wchar_t);
		CHECK_RVALUE_REFERENCE_TYPE(char16_t);
		CHECK_RVALUE_REFERENCE_TYPE(char32_t);
		CHECK_RVALUE_REFERENCE_TYPE(short);
		CHECK_RVALUE_REFERENCE_TYPE(unsigned short);
		CHECK_RVALUE_REFERENCE_TYPE(int);
		CHECK_RVALUE_REFERENCE_TYPE(unsigned int);
		CHECK_RVALUE_REFERENCE_TYPE(long);
		CHECK_RVALUE_REFERENCE_TYPE(unsigned long);
		CHECK_RVALUE_REFERENCE_TYPE(long long);
		CHECK_RVALUE_REFERENCE_TYPE(unsigned long long);
		CHECK_RVALUE_REFERENCE_TYPE(float);
		CHECK_RVALUE_REFERENCE_TYPE(float);
		CHECK_RVALUE_REFERENCE_TYPE(float);
		CHECK_RVALUE_REFERENCE_TYPE(float);
		CHECK_RVALUE_REFERENCE_TYPE(double);
		CHECK_RVALUE_REFERENCE_TYPE(double);
		CHECK_RVALUE_REFERENCE_TYPE(double);
		CHECK_RVALUE_REFERENCE_TYPE(double);
		CHECK_RVALUE_REFERENCE_TYPE(long double);
		CHECK_RVALUE_REFERENCE_TYPE(long double);
		CHECK_RVALUE_REFERENCE_TYPE(long double);
		CHECK_RVALUE_REFERENCE_TYPE(long double);
		str = "TestStr";
		gv = GVariant::create<std::string&&>(std::move(str));
		assert(GVariant::cast<std::string&&>(gv) == std::string("TestStr"));

		CHECK_CONST_LVALUE_REFERENCE_TYPE(bool);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(char);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(signed char);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(unsigned char);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(wchar_t);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(char16_t);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(char32_t);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(short);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(unsigned short);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(int);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(unsigned int);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(long);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(unsigned long);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(long long);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(unsigned long long);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(float);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(float);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(float);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(float);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(double);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(double);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(double);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(double);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(long double);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(long double);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(long double);
		CHECK_CONST_LVALUE_REFERENCE_TYPE(long double);
		str = "TestStr";
		const std::string& str_ref = str;
		gv = GVariant::create<const std::string&>(str_ref);
		str = "modified";
		assert(GVariant::cast<const std::string&>(gv) == std::string("modified"));

		CHECK_CONST_RVALUE_REFERENCE_TYPE(bool);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(char);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(signed char);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(unsigned char);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(wchar_t);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(char16_t);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(char32_t);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(short);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(unsigned short);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(int);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(unsigned int);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(long);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(unsigned long);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(long long);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(unsigned long long);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(float);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(float);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(float);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(float);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(double);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(double);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(double);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(double);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(long double);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(long double);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(long double);
		CHECK_CONST_RVALUE_REFERENCE_TYPE(long double);
		str = "TestStr";
		gv = GVariant::create<const std::string&&>(std::move(str));
		assert(GVariant::cast<const std::string&&>(gv) == std::string("TestStr"));

		cout << "Ok" << endl;
	}

	//compound types - pointers
	{
#define CHECK_POINTER_TYPE(TYPE) { TYPE val = numeric_limits<TYPE>::max(); gv = GVariant::create<TYPE*>(&val); assert(*GVariant::cast<TYPE*>(gv) == val); }

		cout << "Testing GVariant for fundemental type pointers: ";
		GVariant gv;
		void* void_ptr = nullptr;

		//explicit type specification
		gv = GVariant::create<void*>(void_ptr);
		assert(GVariant::cast<void*>(gv) == nullptr);

		gv = GVariant::create<nullptr_t*>(nullptr);
		assert(GVariant::cast<nullptr_t*>(gv) == nullptr);

		CHECK_POINTER_TYPE(bool);
		CHECK_POINTER_TYPE(char);
		CHECK_POINTER_TYPE(signed char);
		CHECK_POINTER_TYPE(unsigned char);
		CHECK_POINTER_TYPE(wchar_t);
		CHECK_POINTER_TYPE(char16_t);
		CHECK_POINTER_TYPE(char32_t);
		CHECK_POINTER_TYPE(short);
		CHECK_POINTER_TYPE(unsigned short);
		CHECK_POINTER_TYPE(int);
		CHECK_POINTER_TYPE(unsigned int);
		CHECK_POINTER_TYPE(long);
		CHECK_POINTER_TYPE(unsigned long);
		CHECK_POINTER_TYPE(long long);
		CHECK_POINTER_TYPE(unsigned long long);
		CHECK_POINTER_TYPE(float);
		CHECK_POINTER_TYPE(float);
		CHECK_POINTER_TYPE(float);
		CHECK_POINTER_TYPE(float);
		CHECK_POINTER_TYPE(double);
		CHECK_POINTER_TYPE(double);
		CHECK_POINTER_TYPE(double);
		CHECK_POINTER_TYPE(double);
		CHECK_POINTER_TYPE(long double);
		CHECK_POINTER_TYPE(long double);
		CHECK_POINTER_TYPE(long double);
		CHECK_POINTER_TYPE(long double);

		CHECK_POINTER_TYPE(const bool);
		CHECK_POINTER_TYPE(const char);
		CHECK_POINTER_TYPE(const signed char);
		CHECK_POINTER_TYPE(const unsigned char);
		CHECK_POINTER_TYPE(const wchar_t);
		CHECK_POINTER_TYPE(const char16_t);
		CHECK_POINTER_TYPE(const char32_t);
		CHECK_POINTER_TYPE(const short);
		CHECK_POINTER_TYPE(const unsigned short);
		CHECK_POINTER_TYPE(const int);
		CHECK_POINTER_TYPE(const unsigned int);
		CHECK_POINTER_TYPE(const long);
		CHECK_POINTER_TYPE(const unsigned long);
		CHECK_POINTER_TYPE(const long long);
		CHECK_POINTER_TYPE(const unsigned long long);
		CHECK_POINTER_TYPE(const float);
		CHECK_POINTER_TYPE(const float);
		CHECK_POINTER_TYPE(const float);
		CHECK_POINTER_TYPE(const float);
		CHECK_POINTER_TYPE(const double);
		CHECK_POINTER_TYPE(const double);
		CHECK_POINTER_TYPE(const double);
		CHECK_POINTER_TYPE(const double);
		CHECK_POINTER_TYPE(const long double);
		CHECK_POINTER_TYPE(const long double);
		CHECK_POINTER_TYPE(const long double);
		CHECK_POINTER_TYPE(const long double);

		cout << "Ok" << endl;
	}

	//objects - value and reference 
	{
		cout << "Testing GVariant for class types and references: ";
		GVariant gv;
		RGB brown = { 255, 128, 0 };

		//lvalue
		gv = GVariant::create<RGB>(brown);
		brown.blue = 32;
		assert(GVariant::cast<RGB>(gv).red == 255);
		assert(GVariant::cast<RGB>(gv).green == 128);
		assert(GVariant::cast<RGB>(gv).blue == 0);

		//rvalue
		gv = GVariant::create<RGB>({ 255, 128, 0 });
		assert(GVariant::cast<RGB>(gv).red == 255);
		assert(GVariant::cast<RGB>(gv).green == 128);
		assert(GVariant::cast<RGB>(gv).blue == 0);

		//lvalue reference
		gv = GVariant::create<RGB&>(brown);
		brown.blue = 0;
		assert(GVariant::cast<RGB&>(gv).red == 255);
		assert(GVariant::cast<RGB&>(gv).green == 128);
		assert(GVariant::cast<RGB&>(gv).blue == 0);

		//rvalue reference
		gv = GVariant::create<RGB&&>({ 255, 128, 0 });
		assert(GVariant::cast<RGB&&>(gv).red == 255);
		assert(GVariant::cast<RGB&&>(gv).green == 128);
		assert(GVariant::cast<RGB&&>(gv).blue == 0);
		cout << "Ok" << endl;
	}

	//pointers to objects
	{
		cout << "Testing GVariant for class type pointers: ";
		GVariant gv;
		RGB brown = { 255, 128, 0 };
		RGB* brown_ptr = &brown;
		const RGB* const_brown_ptr = &brown;

		//non const pointer
		gv = GVariant::create<RGB*>(brown_ptr);
		brown.blue = 32;
		assert(GVariant::cast<RGB*>(gv)->red == 255);
		assert(GVariant::cast<RGB*>(gv)->green == 128);
		assert(GVariant::cast<RGB*>(gv)->blue == 32);

		//const pointer
		gv = GVariant::create<const RGB*>(const_brown_ptr);
		brown.blue = 64;
		assert(GVariant::cast<const RGB*>(gv)->red == 255);
		assert(GVariant::cast<const RGB*>(gv)->green == 128);
		assert(GVariant::cast<const RGB*>(gv)->blue == 64);
		cout << "Ok" << endl;
	}

	//pointers to functions
	{
		cout << "Testing GVariant for pointer to non-class functions: ";
		GVariant gv;
		typedef int(*func_ptr)(int, int);
		func_ptr f = add;

		//function pointer
		gv = GVariant::create<func_ptr>(f);
		assert(GVariant::cast<func_ptr>(gv)(3, 2) == 5);

		//pointer to function pointer
		gv = GVariant::create<func_ptr*>(&f);
		assert((*GVariant::cast<func_ptr*>(gv))(3, 2) == 5);
		f = sub;
		assert((*GVariant::cast<func_ptr*>(gv))(3, 2) == 1);
		cout << "Ok" << endl;
	}

	//pointer to object public members
	{
		cout << "Testing GVariant for pointer to class members data: ";
		GVariant gv;
		RGB brown = { 255, 128, 0 };
		RGB* brown_ptr = &brown;
		const RGB* const_brown_ptr = &brown;

		unsigned char  RGB::* red_ptr = &RGB::red;
		brown.*red_ptr = 20;
		gv = GVariant::create<unsigned char RGB::*>(red_ptr);
		assert(brown.*GVariant::cast<unsigned char RGB::*>(gv) == 20);
		cout << "Ok" << endl;
	}

	//pointer to object public member functions
	{
		cout << "Testing GVariant for pointer to class member functions: ";
		GVariant gv;
		base_class bc;
		derived_class dc;
		base_class& dbc = dc;

		//general member function
		typedef int (derived_class::* general_func_ptr)(void);
		general_func_ptr derive_only_func_ptr = &derived_class::derive_only_func;
		gv = GVariant::create<general_func_ptr>(derive_only_func_ptr);
		assert((dc.*GVariant::cast<general_func_ptr>(gv))() == 44);

		//lvalue ref qualified member function
		typedef int (derived_class::* lvalue_ref_qualified_func_ptr)(void)&;
		lvalue_ref_qualified_func_ptr lref_qualified_member_func_ptr = &derived_class::lref_qualified_member_func;
		gv = GVariant::create<lvalue_ref_qualified_func_ptr>(lref_qualified_member_func_ptr);
		assert((dc.*GVariant::cast<lvalue_ref_qualified_func_ptr>(gv))() == 77);

		//rvalue ref qualified member function
		typedef int (derived_class::* rvalue_ref_qualified_func_ptr)(void)&&;
		rvalue_ref_qualified_func_ptr rref_qualified_member_func_ptr = &derived_class::rref_qualified_member_func;
		gv = GVariant::create<rvalue_ref_qualified_func_ptr>(rref_qualified_member_func_ptr);
		assert((std::move(dc).*GVariant::cast<rvalue_ref_qualified_func_ptr>(gv))() == 88);

		//virtual member function
		typedef int (derived_class::* derived_virtual_func_ptr)(void);
		typedef int (base_class::* base_virtual_func_ptr)(void);
		derived_virtual_func_ptr derived_virtual_func = &derived_class::get_virtual_id;
		base_virtual_func_ptr base_virtual_func = &base_class::get_virtual_id;

		gv = GVariant::create<base_virtual_func_ptr>(base_virtual_func);
		assert((bc.*GVariant::cast<base_virtual_func_ptr>(gv))() == 11);

		gv = GVariant::create<derived_virtual_func_ptr>(derived_virtual_func);
		assert((dc.*GVariant::cast<derived_virtual_func_ptr>(gv))() == 22);

		gv = GVariant::create<base_virtual_func_ptr>(base_virtual_func);
		assert((dbc.*GVariant::cast<base_virtual_func_ptr>(gv))() == 22);

		gv = GVariant::create<base_virtual_func_ptr>(base_virtual_func);
		assert((dc.*GVariant::cast<base_virtual_func_ptr>(gv))() == 22);

		//non virtual function
		typedef int (derived_class::* common_non_virtual_func_ptr)(void);
		common_non_virtual_func_ptr common_non_virtual_func = &derived_class::common_non_virtual_func;
		gv = GVariant::create<common_non_virtual_func_ptr>(common_non_virtual_func);
		assert((dc.*GVariant::cast<common_non_virtual_func_ptr>(gv))() == 66);
		cout << "Ok" << endl;
	}

	//array types
	{
		cout << "Testing GVariant for array types: ";
		GVariant gv;

		gv = GVariant::create<int[]>(int_array);
		assert(GVariant::cast<int*>(gv)[0] == 1);
		assert(GVariant::cast<int*>(gv)[1] == 2);
		assert(GVariant::cast<int*>(gv)[2] == 3);
		cout << "Ok" << endl;
	}

	//class types
	{
		cout << "Testing GVariant for pointer class objects: ";
		GVariant gv;
		non_copyable_class ncc;
		non_moveable_class nmc;

		//gv = GVariant::create<non_copyable_class>(ncc); //expected compile error

		gv = GVariant::create<non_copyable_class&>(ncc);
		ncc.set_number(17);
		assert(GVariant::cast<non_copyable_class&>(gv).get_number() == 17);

		gv = GVariant::create<const non_copyable_class&>(ncc);
		ncc.set_number(22);
		assert(GVariant::cast<const non_copyable_class&>(gv).get_number() == 22);

		gv = GVariant::create<non_copyable_class&&>(std::move(ncc));
		ncc.set_number(17);
		assert(GVariant::cast<non_copyable_class&&>(gv).get_number() == 17);

		gv = GVariant::create<non_copyable_class&&>(non_copyable_class());
		assert(GVariant::cast<non_copyable_class&&>(gv).get_number() == 14);

		gv = GVariant::create<const non_copyable_class&&>(std::move(ncc));
		ncc.set_number(17);
		assert(GVariant::cast<const non_copyable_class&&>(gv).get_number() == 17);

		gv = GVariant::create<const non_copyable_class&&>(non_copyable_class());
		assert(GVariant::cast<const non_copyable_class&&>(gv).get_number() == 14);

		gv = GVariant::create<non_copyable_class*>(&ncc);
		ncc.set_number(37);
		assert(GVariant::cast<non_copyable_class*>(gv)->get_number() == 37);

		gv = GVariant::create<const non_copyable_class*>(&ncc);
		ncc.set_number(42);
		assert(GVariant::cast<const non_copyable_class*>(gv)->get_number() == 42);

		//gv = GVariant::create<non_moveable_class>(nmc); //expected compile error

		gv = GVariant::create<non_moveable_class&>(nmc);
		nmc.set_number(17);
		assert(GVariant::cast<non_moveable_class&>(gv).get_number() == 17);

		gv = GVariant::create<const non_moveable_class&>(nmc);
		nmc.set_number(22);
		assert(GVariant::cast<const non_moveable_class&>(gv).get_number() == 22);

		gv = GVariant::create<non_moveable_class*>(&nmc);
		nmc.set_number(37);
		assert(GVariant::cast<non_moveable_class*>(gv)->get_number() == 37);

		gv = GVariant::create<const non_moveable_class*>(&nmc);
		nmc.set_number(42);
		assert(GVariant::cast<const non_moveable_class*>(gv)->get_number() == 42);
		cout << "Ok" << endl;
	}

#if 0
	//string types
	{
		//std::string
		gv = string("NodeA");
		std::cout << "std::string: " << GVariant::cast<std::string>(gv) << endl;

		//std::string reference
		string str = string("NodeA");
		string& str_ref = str;
		gv = GVariant::ref<string>(str_ref);
		string& new_ref = gv;
		std::cout << "std::string reference: " << new_ref << endl;
		new_ref = string("modifed NodeA");
		std::cout << "std::string reference modified: " << str << endl;
	}

	//pointer
	{
		float i = 2.0f;
		float* ip = &i;
		gv = ip;
		float* ipc = GVariant::cast<float*>(gv);
		std::cout << "value of int pointer: " << *ipc << endl;
		i = 33;
		assert(*ipc == 33.0f);
		std::cout << "value of int pointer: " << *ipc << endl;
	}

	//GVariant inside GVariant
	{
		int i = 4;
		GVariant gv2 = i;
		gv = gv2;
		assert(GVariant::cast<int>(gv) == 4);
	}

	//const pointer
	{
		int i = 2;
		const int* ip = &i;
		gv = ip;
		const int* ipc = GVariant::cast<const int*>(gv);
		std::cout << "value of const int pointer: " << *ipc << endl;
		i = 33;
		std::cout << "value of const int pointer: " << *ipc << endl;
	}

	//array
	{
		int i[3];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;

		gv = i;

		i[0] = 10;
		i[1] = 11;
		i[2] = 12;

		int* ipc = GVariant::cast<int*>(gv);
		assert(*(ipc + 0) == 10);
		assert(*(ipc + 1) == 11);
		assert(*(ipc + 2) == 12);
	}

	//reference
	{
		float i = 2.0f;
		float& ip = i;
		gv = GVariant::ref<float>(ip);
		float& ipc = GVariant::cast<float&>(gv);
		std::cout << "value of float reference: " << ipc << endl;
		i = 33;
		assert(ipc == 33.0f);
		std::cout << "value of float reference: " << ipc << endl;
	}

	//const reference
	{
		float i = 2.0f;
		const float& ip = i;
		gv = GVariant::ref<const float>(ip);
		const float& ipc = GVariant::cast<const float&>(gv);
		std::cout << "value of const float reference: " << ipc << endl;
		i = 33.0f;
		assert(ipc == 33.0f);
		std::cout << "value of const float reference: " << ipc << endl;

	}

	//class object
	{
		boost::any a = sphere_node;
		gv = sphere_node;
		assert(GVariant::cast<sphere>(gv).getRadius() == 64);
	}

	//class object reference
	{
		sphere& sref = sphere_node;
		gv = GVariant::ref<sphere>(sref);
		sphere_node.setRadius(96);
		assert(GVariant::cast<sphere&>(gv).getRadius() == 96);
		sphere_node.setRadius(64);
	}

	//class object const reference
	{
		const sphere& sref = sphere_node;
		gv = GVariant::ref<const sphere>(sref);
		sphere_node.setRadius(56);
		assert(GVariant::cast<const sphere&>(gv).getRadius() == 56);
		sphere_node.setRadius(64);
	}

	//class object that cannot be copied
	{
		//TODO: design a test case to test how GVariant can store objects that cannot be copied
	}

	//class raw pointer
	{
		gv = parent_node_sharedptr.get();
		parent_node_sharedptr->setRadius(73);
		assert(GVariant::cast<sphere*>(gv)->getRadius() == 73);
		parent_node_sharedptr->setRadius(32);
	}

	//class shared pointer
	{
		gv = parent_node_sharedptr;
		assert(GVariant::cast<SphereSharedPtr>(gv)->getRadius() == 32);
	}

	//passing variant as argument to a function taking POD
	GVariant a1, a2, a3;
	a1 = true;
	a2 = 10;
	a3 = 3.14f;

	//print_data(a1, a2, a3);


	//passing direct POD types to a function taking variant arguments
	//print_data(false, 10, 3.14);
	string st("ss");
	a3 = st;
	std::cout << boost::core::demangled_name(a3.type()) << std::endl;
	std::cout << typeid(st).name() << std::endl;

	float i = 3.0;
	gv = i;
	// the below line throws exception if uncommented
	//int f = boost::get<int>(gv);
#endif
}
#endif
namespace 
{
	GVariant gv;
	char* \U000000AF = "cat"; // supported
}
#define GTEST_FUNDEMENTAL_TYPE( SUITE, CATEGORY, TYPE ) \
	TEST(SUITE##$$$##CATEGORY, TYPE ) { \
		EXPECT_EQ(test_gvariant_fundemental_types<TYPE >(gv), true); \
	}

GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, void)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, nullptr_t)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, bool)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, int8)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, uint8)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, char)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, char16_t)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, char32_t)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, int16)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, uint16)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, int32)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, uint32)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, int64)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, uint64)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, float32)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, float64)
GTEST_FUNDEMENTAL_TYPE(GVariantTest, FundementalTypes, float80)

#define GTEST_COMPOUND_TYPE( SUITE, CATEGORY, TYPE ) \
	TEST(SUITE##$$$##CATEGORY, TYPE ) { \
		EXPECT_EQ(test_gvariant_compund_types_##TYPE(gv), true); \
	}

GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, raw_pointer)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, one_dim_array)
GTEST_COMPOUND_TYPE(DISABLED_GVariantTest, CompoundTypes, multi_dim_array)
GTEST_COMPOUND_TYPE(DISABLED_GVariantTest, CompoundTypes, shared_ptr)
GTEST_COMPOUND_TYPE(DISABLED_GVariantTest, CompoundTypes, unique_ptr)
GTEST_COMPOUND_TYPE(DISABLED_GVariantTest, CompoundTypes, lvalue_ref)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, rvalue_ref)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, member_variable_pointer)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, member_function_pointer)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, class)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, function)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, enum)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, union)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, GVariant)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, string)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, wstring)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, vector)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, list)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, map)
GTEST_COMPOUND_TYPE(GVariantTest, CompoundTypes, tuple)

#define GTEST_GPROPERTY_TYPE( SUITE, CATEGORY, PROP_TYPE ) \
	TEST(SUITE##$$$##CATEGORY, PROP_TYPE ) { \
		EXPECT_EQ( (test_gvariant_gproperty_types<PROP_TYPE, PROP_TYPE::type>(gv)), true); \
	}


GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GBoolProperty)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GCharProperty)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GInt8Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GUint8Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GInt16Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GUint16Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GInt32Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GUint32Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GInt64Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GUint64Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GFloatProperty)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GDoubleProperty)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GVec2Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GVec3Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GVec4Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GMat2Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GMat3Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GMat4Property)
GTEST_GPROPERTY_TYPE(GVariantTest, GProperty, GObjectPointerProperty)

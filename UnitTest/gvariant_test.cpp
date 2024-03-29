#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <array>
#include <stack>
#include <map>
#include <cassert>
#include <limits>
#include <cmath>
#include <array>
#include <functional>

#include <gtest/gtest.h>

#include <GFramework/GVariant/GTypes.h>
#include <GFramework/GVariant/GVariant.h>
#include <GFramework/GVariant/GObject.h>

#include "gvariant_test.h"


using namespace std;
using namespace GFramework;

template<typename T>
struct is_vector : std::false_type {};

template<typename T>
struct is_vector<std::vector<T>> : std::true_type {};

template<typename T>
struct is_list : std::false_type {};

template<typename T>
struct is_list<std::list<T>> : std::true_type {};

template<typename T>
struct is_stack : std::false_type {};

template<typename T, typename C>
struct is_stack<std::stack<T, C>> : std::true_type {};

template<typename T>
struct is_map : std::false_type {};

template<class K, class T, class C, class A>
struct is_map<std::map<K, T, C, A>> : std::true_type {};

template<typename T>
struct is_tuple : std::false_type {};

template<class... Types>
struct is_tuple<std::tuple<Types...>> : std::true_type {};

template<typename T>
struct is_function_object : std::false_type {};

template<typename T>
struct is_function_object<std::function<T>> : std::true_type {};

template<typename T, typename U = T>
typename T::DUMMY_TYPE
gvariant_arithmetic_test(GVariant);

template<typename T, typename U = T>
typename T::DUMMY_TYPE
gvariant_floatingpoint_test(GVariant);

template<typename T>
typename T::DUMMY_TYPE
gvariant_class_test(GVariant, T);

template<typename T>
bool gvariant_pointer_test(GVariant gv, T v1)
{
	bool result = true;

	//raw pointer
	T* v6 = &v1;
	gv = GVariant::create<T*>(v6);
	result = result && (GVariant::cast<T*>(gv) == v6);
	result = result && (*GVariant::cast<T*>(gv) == v1);
	return result;
}

template<typename T>
bool gvariant_reference_test(GVariant gv, T v1)
{
	bool result = true;

	//lvalue reference
	T& v2 = v1;
	gv = GVariant::create<T>(v2);
	result = result && (GVariant::cast<T>(gv) == v2);

	//const lvalue
	const T v3 = v1;
	gv = GVariant::create<T>(v3);
	result = result && (GVariant::cast<T>(gv) == v3);

	//const lvalue reference
	const T& v4 = v1;
	gv = GVariant::create<T>(v4);
	result = result && (GVariant::cast<T>(gv) == v4);

	//rvalue
	auto ret_rvalue = [v1]()->T {T v = v1; return v; };
	gv = GVariant::create<T>(ret_rvalue());
	result = result && (GVariant::cast<T>(gv) == ret_rvalue());

	//rvalue reference
	T&& v5 = std::move(v1);
	gv = GVariant::create<T>(v5);
	result = result && (GVariant::cast<T>(gv) == v5);

	result = result && gvariant_pointer_test<T>(gv, v1);

	return result;
}

template<typename T, typename U = T>
typename std::enable_if<std::is_arithmetic<U>::value, bool>::type
gvariant_arithmetic_test(GVariant gv)
{
	bool result = true;

	//constexpr range
	gv = GVariant::create<T>(numeric_limits<U>::min());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::min());

	gv = GVariant::create<T>(numeric_limits<U>::lowest());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::lowest());

	gv = GVariant::create<T>(numeric_limits<U>::max());
	result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::max());

	//lvalue
	T v1 = numeric_limits<U>::min();
	gv = GVariant::create<T>(v1);
	result = result && (GVariant::cast<T>(gv) == v1);

	result = result && gvariant_reference_test<T>(gv, v1);

	return result;
}

template<typename T, typename U = T>
typename std::enable_if<std::is_floating_point<U>::value, bool>::type
gvariant_floatingpoint_test(GVariant gv)
{
	bool result = true;

	result = result && gvariant_arithmetic_test<T, U>(gv);

	if (numeric_limits<U>::has_infinity)
	{
		gv = GVariant::create<T>(numeric_limits<U>::infinity());
		result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::infinity());
	}

	if (numeric_limits<U>::has_quiet_NaN)
	{
		gv = GVariant::create<T>(numeric_limits<U>::quiet_NaN());
		result = result && (std::isnan((U)GVariant::cast<T>(gv))); //NaN is always != Nan, use std::isnan
	}

	if (numeric_limits<U>::has_signaling_NaN)
	{
		gv = GVariant::create<T>(numeric_limits<U>::signaling_NaN());
		result = result && (std::isnan((U)GVariant::cast<T>(gv))); //NaN is always != Nan, use std::isnan
	}

	if (numeric_limits<U>::has_denorm)
	{
		gv = GVariant::create<T>(numeric_limits<U>::denorm_min());
		result = result && (GVariant::cast<T>(gv) == numeric_limits<U>::denorm_min());
	}

	return result;
}

template<typename T>
typename std::enable_if<std::is_class<T>::value, bool>::type
gvariant_class_test(GVariant gv, T t)
{
	bool result = true;

	result = result && gvariant_reference_test<T>(gv, t);

	return result;
}

template<typename T, typename = void>
struct dataType {};

template<typename T>
struct dataType <T, typename std::enable_if< std::is_void<T>::value >::type>
{
	static bool variant_test(GVariant gv)
	{
		bool result = true;
		int i = 23;

		//raw void pointer
		void* v6 = static_cast<void*>(&i);
		gv = GVariant::create<void*>(v6);
		result = result && (GVariant::cast<void*>(gv) == v6);
		result = result && (*(static_cast<int*>(GVariant::cast<void*>(gv))) == i);

		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< std::is_null_pointer<T>::value >::type>
{
	static bool variant_test(GVariant gv)
	{
		bool result = true;
		
		//raw null pointer
		gv = GVariant::create<nullptr_t>(nullptr);
		result = result && (GVariant::cast<nullptr_t>(gv) == nullptr);

		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< std::is_integral<T>::value >::type>
{
	static bool variant_test(GVariant gv)
	{
		return gvariant_arithmetic_test<T>(gv);
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< std::is_floating_point<T>::value >::type>
{
	static bool variant_test(GVariant gv)
	{
		bool result = true;
		
		result = result && gvariant_arithmetic_test<T>(gv);

		result = result && gvariant_floatingpoint_test<T>(gv);

		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< std::is_array<T>::value >::type>
{
	static_assert(std::rank<T>::value == 1, "Multi dimensional array is not supported");

	static bool variant_test(GVariant gv, T t)
	{
		bool result = true;
		gv = GVariant::create<T>(t);
		result = result && (GVariant::cast<typename std::decay<T>::type>(gv)[0] == t[0]);
		result = result && (GVariant::cast<typename std::decay<T>::type>(gv)[std::extent<T>::value-1] == t[std::extent<T>::value-1]);
		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< is_vector<T>::value >::type>
{
	static bool variant_test(GVariant gv, T t)
	{
		bool result = true;
		gv = GVariant::create<T>(t);
		result = result && (GVariant::cast<T>(gv) == t);
		if (t.size() > 0)
			result = result && dataType<typename T::value_type>::variant_test(t[0]);
		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< is_list<T>::value >::type>
{
	static bool variant_test(GVariant gv, T t)
	{
		bool result = true;
		gv = GVariant::create<T>(t);
		result = result && (GVariant::cast<T>(gv) == t);
		if (t.size() > 0)
			result = result && dataType<typename T::value_type>::variant_test(t.front());
		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< is_map<T>::value >::type>
{
	static bool variant_test(GVariant gv, T t)
	{
		bool result = true;
		gv = GVariant::create<T>(t);
		result = result && (GVariant::cast<T>(gv) == t);
		if (t.size() > 0) {
			result = result && dataType<typename T::value_type>::variant_test(t.begin()->first);
			result = result && dataType<typename T::value_type>::variant_test(t.begin()->second);
		}
		return result;
	}
};

template<class... Types>
struct dataType <std::tuple<Types...>>
{
	static bool variant_test(GVariant gv, std::tuple<Types...> t)
	{
		bool result = true;
		gv = GVariant::create<std::tuple<Types...>>(t);
		result = result && (GVariant::cast<std::tuple<Types...>>(gv) == t);
		result = result && forEachType<Types...>::variant_test(gv);
		return result;
	}
private:
	template<class U, class... Types_Intenal>
	struct forEachType
	{
		static bool variant_test(GVariant gv)
		{
			bool result = dataType<U>::variant_test(gv);
			result = result && forEachType<Types_Intenal...>::variant_test(gv);
			return result;
		}
	};

	template<class U>
	struct forEachType<U>
	{
		static bool variant_test(GVariant gv)
		{
			return dataType<U>::variant_test(gv);
		}
	};
};


template<typename T>
struct dataType <T, typename std::enable_if< is_function_object<T>::value >::type>
{
	template<typename R>
	static bool variant_test(GVariant gv, T t, R r)
	{
		bool result = true;

		gv = GVariant::create(t);
		result = result && ((GVariant::cast<T>(gv))() == r);

		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< is_shared_ptr<T>::value >::type>
{
	static bool variant_test(GVariant gv, T t)
	{
		bool result = true;
		gv = GVariant::create(t);
		result = result && ((GVariant::cast<T>(gv)) == t);

		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if < std::is_class<T>::value && !	(	is_function_object<T>::value ||
																				std::is_base_of<GPropertyInterface, T>::value ||
																				is_vector<T>::value ||
																				is_list<T>::value ||
																				std::is_array<T>::value ||
																				is_stack<T>::value ||
																				is_map<T>::value ||
																				is_tuple<T>::value ||
																				is_shared_ptr<T>::value
																			)>::type>
{
	static bool variant_test(GVariant gv, T t=T())
	{
		bool result = true;

		gv = GVariant::create(t);
		result = result && (GVariant::cast<T>(gv) == t);
		result = gvariant_reference_test<T>(gv, t);

		return result;
	}
};

template<typename T>
struct dataType <T, typename std::enable_if< std::is_base_of<GPropertyInterface, T>::value>::type>
{
	static bool variant_test(GVariant gv)
	{
		bool result = true;
		T t;
		result = result && gvariant_class_test<T>(gv, t);
		result = result && dataType_internal<typename T::type>::variant_test(gv);
		return result;
	}
private:
	template<typename U, typename = void>
	struct dataType_internal {};

	template<typename U>
	struct dataType_internal <U, typename std::enable_if< std::is_integral<U>::value >::type>
	{
		static bool variant_test(GVariant gv)
		{
			return gvariant_arithmetic_test<T, U>(gv);
		}
	};

	template<typename U>
	struct dataType_internal <U, typename std::enable_if< std::is_floating_point<U>::value >::type>
	{
		static bool variant_test(GVariant gv)
		{
			bool result = true;

			result = result && gvariant_arithmetic_test<T, U>(gv);

			result = result && gvariant_floatingpoint_test<T, U>(gv);

			return result;
		}
	};

	template<typename U>
	struct dataType_internal <U, typename std::enable_if< std::is_class<U>::value >::type>
	{
		static bool variant_test(GVariant gv)
		{
			bool result = true;
			U u;
			result = result && gvariant_class_test<U>(gv, u);
			return result;
		}
	};
};

class Color
{
public:
	Color() {}
	Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a) : r(_r), g(_g), b(_b), a(_a) {}
	uint8 r = 0, g = 0, b = 0, a = 0;
	bool operator==(const Color& rhs) { return (this->r == rhs.r && this->g == rhs.g && this->b == rhs.b && this->a == rhs.a); }
};

class GVariantTest$$CompoundTypes : public ::testing::Test {
protected:
	void SetUp() override {
		intVector.push_back(11);
		intVector.push_back(12);
		intVector.push_back(13);

		floatList.push_back(1.1f);
		floatList.push_back(2.2f);
		floatList.push_back(3.3f);

		stringDoubleMap["1.23"] = 1.23;
		stringDoubleMap["4.56"] = 4.56;
		stringDoubleMap["7.89"] = 7.89;

		boolIntStringTuple = std::make_tuple(true, 768, "large number");
	}

protected:
	string str = "Default test string";
	wstring wstr = L"\x6f\x6c\xc3\xa9";
	int arr1[5] = { 1,2,3,4,5 };
	int arr2[2][5] = { { 1,2,3,4,5 }, { 1,2,3,4,5 } };
	glm::vec3 arr3[3] = { glm::vec3(1),glm::vec3(2), glm::vec3(3) };
	std::vector<int> intVector;
	std::list<float> floatList;
	std::map < std::string,  double> stringDoubleMap;
	std::tuple<bool, int, std::string > boolIntStringTuple;
	Color red{ 255,0,0,255 };
	std::shared_ptr<Color> green = std::make_shared<Color>(0, 255, 0, 255);
};

GVariant gv;

#define GTEST_GVARIANT2( SUITE, NAME, TYPE ) \
	TEST(SUITE, NAME ) { \
		EXPECT_EQ(true, dataType<TYPE>::variant_test(gv)); \
	}

#define GTEST_GVARIANT( SUITE, TYPE ) GTEST_GVARIANT2(SUITE, TYPE, TYPE)


GTEST_GVARIANT(GVariantTest$$FundementalTypes, void)
GTEST_GVARIANT2(GVariantTest$$FundementalTypes, nullptrType, nullptr_t)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, bool)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, int8)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, uint8)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, char)
GTEST_GVARIANT2(GVariantTest$$FundementalTypes, char16Type, char16_t)
GTEST_GVARIANT2(GVariantTest$$FundementalTypes, char32Type, char32_t)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, int16)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, uint16)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, int32)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, uint32)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, int64)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, uint64)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, float32)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, float64)
GTEST_GVARIANT(GVariantTest$$FundementalTypes, float80)

TEST_F(GVariantTest$$CompoundTypes, OneDimensionalArray) {
	EXPECT_EQ(true, dataType<decltype(arr1)>::variant_test(gv, arr1));
	//EXPECT_EQ(true, dataType<decltype(arr2)>::variant_test(gv, arr2)); //Must give compilation error
	EXPECT_EQ(true, dataType<decltype(arr3)>::variant_test(gv, arr3));
}

TEST_F(GVariantTest$$CompoundTypes, FunctionObject) {
	bool result = true;
	std::function<int()> f1 = []()->int {return 345; };
	EXPECT_EQ(true, dataType<decltype(f1)>::variant_test(gv, f1, 345));

	int i = 23;
	std::function<int()> f2 = [=]()->int {return i; };
	EXPECT_EQ(true, dataType<decltype(f2)>::variant_test(gv, f2, i));

	int j = 47;
	std::function<int()> f3 = [&]()->int {return ++j; };
	EXPECT_EQ(true, dataType<decltype(f3)>::variant_test(gv, f3, 48));
}

TEST_F(GVariantTest$$CompoundTypes, Class) {
	EXPECT_EQ(true, dataType<Color>::variant_test(gv, red));
}

TEST_F(GVariantTest$$CompoundTypes, SharedPtr) {
	EXPECT_EQ(true, dataType< std::shared_ptr<Color> >::variant_test(gv, green));
}

TEST_F(GVariantTest$$CompoundTypes, string) {
	EXPECT_EQ(true, dataType< string >::variant_test(gv, str));
}

TEST_F(GVariantTest$$CompoundTypes, wstring) {
	EXPECT_EQ(true, dataType< wstring >::variant_test(gv, wstr));
}


//STL Container types
TEST_F(GVariantTest$$CompoundTypes, Vector) {
	EXPECT_EQ(true, dataType< std::vector<int>>::variant_test(gv, intVector));
}

TEST_F(GVariantTest$$CompoundTypes, List) {
	EXPECT_EQ(true, dataType< std::list<float>>::variant_test(gv, floatList));
}

TEST_F(GVariantTest$$CompoundTypes, Map) {
	bool result = dataType< std::map < std::string, double> >::variant_test(gv, stringDoubleMap);
	EXPECT_EQ(true, result);
}

TEST_F(GVariantTest$$CompoundTypes, Tuple) {
	bool result = dataType< std::tuple<bool, int, std::string >  >::variant_test(gv, boolIntStringTuple);
	EXPECT_EQ(true, result);
}

TEST(DISABLED_GVariantTest$$CompoundTypes, Multi_Dim_Array) {
	bool result = false;
	EXPECT_EQ(true, result);
}

TEST(DISABLED_GVariantTest$$CompoundTypes, unique_ptr) {
	bool result = false;
	EXPECT_EQ(true, result);
}

TEST(DISABLED_GVariantTest$$CompoundTypes, lvalue_ref) {
	bool result = false;
	EXPECT_EQ(true, result);
}

//GProperty types
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GBoolProperty)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GCharProperty)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GInt8Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GUint8Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GInt16Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GUint16Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GInt32Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GUint32Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GInt64Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GUint64Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GFloatProperty)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GDoubleProperty)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GVec2Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GVec3Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GVec4Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GMat2Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GMat3Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GMat4Property)
GTEST_GVARIANT(GVariantTest$$GPropertyTypes, GObjectPointerProperty)


//Serialization test
//GTEST_GSERIALIZATION(GVariantTest$$FundementalTypes, int8)

void variant_test()
{

}

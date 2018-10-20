#include <iostream>
#include <cassert>
#include <limits>
#include <boost/core/typeinfo.hpp>
#include <GVariant.h>
/*
#include <Node.h>
#include <Sphere.h>
*/
#include "gvariant_test.h"
#include <boost\variant.hpp>
#include <boost\function.hpp>
#include <array>
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
	non_copyable_class(const non_copyable_class &) = delete;
	non_copyable_class & operator=(const non_copyable_class &) = delete;
};

class non_moveable_class {
public:
	non_moveable_class(non_moveable_class&&) = delete;
};

class base_class {
public:
	static float get_static_PI() {
		return 3.14;
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

	int lref_qualified_member_func() & {
		return 77;
	}

	int rref_qualified_member_func() && {
		return 88;
	}
};

void process_base_class(base_class* bc) {

}

typedef int(*non_member_func_ptr(float));

typedef int(base_class::*virtual_member_func_ptr(void));

typedef int(derived_class::*nonvirtual_member_func_ptr(void));

void run_variant_testcases()
{
	/*
	std::cout << endl << "Starting test cases for 'Variant'..." << endl << endl;
	NodeSharedPtr grant_parent_node_sharedptr(std::make_shared<Node>("grant_parent", NodeSharedPtr()));
	SphereSharedPtr parent_node_sharedptr(std::make_shared<sphere>("parent_node", grant_parent_node_sharedptr, 32));
	sphere sphere_node("sphere_node", static_pointer_cast<Node>(parent_node_sharedptr), 64);
	*/
	
	//fundamental types
	{
		GVariant gv;
#if 1
		//explicit type specification
		gv = GVariant::create<void>();

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
#endif
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
		GVariant gv;
#if 1
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
#endif
	}

	//compound types - pointers
	{
#define CHECK_POINTER_TYPE(TYPE) { TYPE val = numeric_limits<TYPE>::max(); gv = GVariant::create<TYPE*>(&val); assert(*GVariant::cast<TYPE*>(gv) == val); }

	GVariant gv;
	void* void_ptr = nullptr;
#if 1
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
#endif
	}

	//objects
	{
		GVariant gv;
		RGB brown = { 255, 128, 0 };

		gv = GVariant::create<RGB>(brown);
		assert(GVariant::cast<RGB>(gv).red == 255);
		assert(GVariant::cast<RGB>(gv).green == 128);
		assert(GVariant::cast<RGB>(gv).blue == 0);
	}

	//pointers to objects
	{

	}

	//pointers to functions
	{

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
		assert(*(ipc+0) == 10);
		assert(*(ipc+1) == 11);
		assert(*(ipc+2) == 12);
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
	//rough();
	
}



#if 0
void tee(int i, float f, bool b)
{
	cout << i << "," << f << "," << b << endl;
}


template<typename FUNC, typename Array, std::size_t... I>
ResultType<FUNC> helper(FUNC f, const Array& a, std::index_sequence<I...>)
{
	//tee(a[I]...);
	return f(a[I]...);
}

template<typename FUNC, typename Array>
ResultType<FUNC> metahelper(FUNC f, const Array& a)
{
	return helper(f, a, std::make_index_sequence<Arity<FUNC>::value>());
}

void rough()
{
	//pr(doo);
	//createCmd("foo", foo);
	//ArgPrinter(doo);
	//doo(1, 2, 3);
	GVariant v1, v2, v3;

	double d = 9.0;
	v1 = 2;
	v2 = 3.14f;
	v3 = true;

	array<GVariant, 3> arr;

	arr[0] = v1;
	arr[1] = v2;
	arr[2] = v3;

	//metahelper(tee, arr);
	int h = v1;

	tee(v1, v2, v3);

}
#endif
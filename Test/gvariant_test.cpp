#include <iostream>
#include <boost/core/typeinfo.hpp>
#include <GVariant.h>
#include <Node.h>
#include <Sphere.h>
#include "gvariant_test.h"
#include <boost\variant.hpp>
#include <boost\function.hpp>
#include <array>
using namespace std;
using namespace GFramework;

/*	Notes:
	> using boost::get<T>() is preferred, simply using the GVariant may give issues
	> Definig a base class in the variant's templaes can accept derived class. But that base class SHOULD NOT be a abstract class; But base class pointer is acceptable.
	> if a reference is added in the variant template, then that variant can be ONLY initialized. Reassignment is not possible.
	> variants are not automatically casted to the actual type
	> cout can automatically resolve the variant type without explicit get<T>.
	> passing direct POD types to a function taking variant arguments works but automatic type cast is not as expected. For ex, 'true' is printed as 1;
*/
void rough();
#if 1
void print_data(bool i, int j, float k)
{
	//cout << i.which();
	std::cout << "Print_data:  " << i << "," << j << "," << k << endl;
}
#endif
void run_variant_testcases()
{
#if 1
	cout << endl << "Starting test cases for 'Variant'..." << endl << endl;

	GVariant gv;

	//simple POD types
	gv = 2;
	cout << "Integer: " << GVariant::cast<int>(gv) << endl;

	gv = 3.14f;
	cout << "float: " << GVariant::cast<float>(gv) << endl;

	gv = 'S';
	cout << "Char: " << GVariant::cast<char>(gv) << endl;

	//std::string
	gv = string("NodeA");
	cout << "std::string: " << GVariant::cast<std::string>(gv) << endl;

	//abstract class pointer
	NodeSharedPtr np(NULL);
	NodeSharedPtr sp = std::make_shared<sphere>("node", np);
	gv = sp;
	cout << "Abstract class pointer: " << GVariant::cast<NodeSharedPtr>(gv)->getName() << endl;

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
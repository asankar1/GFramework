#include <iostream>
#include <cassert>
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
	> Definig a base class in the variant's templaes can accept derived class. But that base class SHOULD NOT be an abstract class; But base class pointer is acceptable.
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

int& add(const int& j)
{
	int i = 3;
	return i;
}

static GVariant call_add()
{
	int j = 0;
	return add(j);
}

void run_variant_testcases()
{
#if 1
	std::cout << endl << "Starting test cases for 'Variant'..." << endl << endl;
	NodeSharedPtr grant_parent_node_sharedptr(std::make_shared<Node>("grant_parent", NodeSharedPtr()));
	SphereSharedPtr parent_node_sharedptr(std::make_shared<sphere>("parent_node", grant_parent_node_sharedptr, 32));
	sphere sphere_node("sphere_node", static_pointer_cast<Node>(parent_node_sharedptr), 64);
	
	GVariant gv;

	//simple POD types
	{
		gv = 4;
		assert(GVariant::cast<int>(gv) == 4);
		std::cout << "int: " << GVariant::cast<int>(gv) << endl;

#if 1
		int h = 9;
		gv = h;
		h = 3;
		assert(GVariant::cast<int>(gv) == 9);

		gv = 3.14f;
		assert(GVariant::cast<float>(gv) == 3.14f);
		std::cout << "float: " << GVariant::cast<float>(gv) << endl;

		gv = 'S';
		assert(GVariant::cast<char>(gv) == 'S');
		std::cout << "Char: " << GVariant::cast<char>(gv) << endl;
#endif
		int j = 5;
		int &rj = j;
		gv = GVariant::ref<int>(rj);
		j = 7;
		assert(GVariant::cast<int&>(gv) == 7);
		std::cout << "int reference: " << GVariant::cast<int&>(gv) << endl;
		
	}

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
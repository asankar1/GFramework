#include <iostream>
#include <GVariant.h>
#include <GReflection.h>
#include <Node.h>
#include <Sphere.h>
#include <boost\optional.hpp>
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include "greflection_test.h"

using namespace std;
using namespace GFramework;

/*	Requirement:
	> get all the reflected variables and functions of a class along with the data types.
	> need an easy way to make the class to register the metadata only once
*/

/*	Notes:
> 
*/

static void print(bool i, int j, float k)
{
	cout << "Print_data:  " << i << "," << j << "," << k << endl;
}

GVariant hi() {
	GVariant v;
	return v;
}

void add2(int &v)
{
	v += 2;
}

template <typename F>
class C
{
public:
	void doOperation(F f)
	{
		int temp = 0;
		//f(temp);
		std::cout << "Result is " << temp << std::endl;
	}
};

template <typename F>
void doOperation(F f)
{
	int temp = 0;
	//f(temp);

	std::cout << typeid(F).name() << "Result is " << f << std::endl;
}

template <typename F>
void invoke(F f)
{
	int temp = 0;	
	boost::function<typename std::remove_reference<std::remove_const<decltype(*f)>::type>::type> _f = f;
	_f(true, 4, 6.8f);
	std::cout << "Result is " << temp << std::endl;
}

struct int_div {
	float operator()(int x, int y) const { return ((float)x) / y; };
};

class H
{
public:
	void hi(float f1, float f2, float f3) {
		cout << "Hello!" << f1 << endl;
	}
};

template <typename F>
void func(F _f)
{
	NodeSharedPtr np(NULL);
	Node n("noood", np);
	typename std::remove_reference<std::remove_const<decltype(_f)>::type>::type f = _f;
	auto nab = boost::bind(f, &n);
	nab();
}

void run_reflection_testcases()
{
	cout << endl << "Starting test cases for 'Reflection'..." << endl << endl;

	GMetaclass *nodemeta = GMetaclassList::instance().getMetaclass("node");
	GMetaclass *spheremeta = GMetaclassList::instance().getMetaclass("sphere");

	cout << "Sphere class functions" << endl;
	std::vector<std::string> f_list;
	std::vector<std::string> p_list;
	//spheremeta->getFunctionsList(f_list);

	GMetaclassList::instance().getMetaclassByType<sphere>().getFunctionsList(f_list);

	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}

	GMetaclassList::instance().getMetaclassByType<sphere>().getEditablePropertiesList(p_list);
	GMetaclassList::instance().getMetaclassByType<sphere>().getPropertiesList(p_list);

	for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
		cout << *it << endl;
	}

	cout << nodemeta->getName() << endl;

	NodeSharedPtr np(NULL);
	NodeSharedPtr parent(new Node("parent",np));
	Node n("noood", parent);
	sphere s("sphere1", parent);

#if 1
	// call function taking void arguments and return void 
	{
		GMetafunction* m = nodemeta->getFunction("about");
		std::vector<GVariant> args;
		m->invoke(&n, args);
	}

	// call function taking void arguments and return void 
	{
		GMetafunction* m = spheremeta->getFunction("about");
		std::vector<GVariant> args;
		m->invoke(&s, args);		
		cout << m->getPrototype() << endl;
	}

	// call function taking non-void arguments and return void
	{
		GMetafunction* m = spheremeta->getFunction("setRadius");
		std::vector<GVariant> args;
		args.push_back(7u);
		m->invoke(&s, args);
		cout << m->getPrototype() << endl;
	}

	// call function taking void arguments but return a value
	{
		GMetafunction* m = spheremeta->getFunction("getRadius");
		std::vector<GVariant> args;
		GVariant rv = m->invoke(&s, args);
		unsigned int r = boost::any_cast<unsigned int>(rv);
		cout << "Radius of sphere: " << s.getName() << " is " << r << endl;
		cout << m->getPrototype() << endl;
	}

	// call function taking shared pointer arguments but return void
	{
		GMetafunction* m = spheremeta->getFunction("setParent");
		std::vector<GVariant> args;
		args.push_back(parent);
		m->invoke(&s, args);
		cout << m->getPrototype() << endl;
	}
	
	// call function taking void arguments but return a shared pointer
	{
		GMetafunction* m = spheremeta->getFunction("getParent");
		std::vector<GVariant> args;
		GVariant rv = m->invoke(&s, args);
		NodeSharedPtr r = boost::any_cast<NodeSharedPtr>(rv);
		cout << "parent of sphere: " << s.getName() << " is " << r->getName() << endl;
		cout << m->getPrototype() << endl;
	}

	//call a function of base class
	{
		GMetafunction* m = spheremeta->getFunction("setPosition");
		std::vector<GVariant> args;
		args.push_back(1.0f);
		args.push_back(2.0f);
		args.push_back(3.0f);
		m->invoke(&s, args);
		cout << m->getPrototype() << endl;
	}

	// call function taking void arguments and return void 
	{
		GMetafunction* m = spheremeta->getFunction("about");
		std::vector<GVariant> args;
		m->invoke(&s, args);
		cout << m->getPrototype() << endl;
	}

	//read member property of type unsigned int
	{
		GMetaproperty *m = spheremeta->getProperty("radius");
		GVariant v = m->get(&s);
		cout << "value of " << m->getName() << " is " << boost::any_cast<unsigned int>(v) << endl;
	}

	//modify member property of type unsigned int
	{
		GMetaproperty *m = spheremeta->getProperty("radius");
		GVariant v = 12U;
		m->set(&s, v);
		cout << "modified the value of " << m->getName() << " to " << boost::any_cast<unsigned int>(v) << endl;
	}

	//read member property of type unsigned int
	{
		GMetaproperty *m = spheremeta->getProperty("radius");
		GVariant v = m->get(&s);
		cout << "value of " << m->getName() << " is " << boost::any_cast<unsigned int>(v) << endl;
	}
#endif
}


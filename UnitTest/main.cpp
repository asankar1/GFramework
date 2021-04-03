#include <iostream>
#ifdef _WINDOWS
#include <conio.h>
#endif
#include <typeindex>
#include <vector>
#include <map>
#include "gvariant_test.h"
#include "gproperty_test.h"
#include "greflection_test.h"

#include "gserialization_test.h"

#include "gscript_test.h"

//#include <GReflection\GReflection.h>
#include <Node.h>

using namespace std;
using namespace GFramework;

//template <class T> __declspec(dllimport) GMetaNonAbstractclass<T>* GMetaNonAbstractclass::getInstance();

//template class __declspec(dllimport) GMetaNonAbstractclass<GFrameworkTest::Node>;

class test
{
public:
	static void get_test() {}
	
};

int main()
{
	//run_variant_testcases();
	//run_property_testcases();
	//run_reflection_testcases();
	run_serialization_testcases();
	//run_script_testcases();
	cout << "\nPress any key to exit...";
	test t;
	t.get_test();
	test::get_test();
#ifdef _WINDOWS
	_getch();
#endif
	return 0;
}	

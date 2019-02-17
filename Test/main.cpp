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
/*
#include "gserialization_test.h"
*/
#include "gscript_test.h"
using namespace std;

int main()
{
	//run_variant_testcases();
	//run_property_testcases();
	run_reflection_testcases();
	//run_serialization_testcases();
	//run_script_testcases();
	//cout << "\nPress any key to exit...";
#ifdef _WINDOWS
	_getch();
#endif
	return 0;
}	

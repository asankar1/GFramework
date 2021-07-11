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

#include <Node.h>

using namespace std;
using namespace GFramework;

int main()
{
	run_variant_testcases();
	/*run_property_testcases();
	run_reflection_testcases();
	run_serialization_testcases();
	run_script_testcases();*/
	cout << "\nPress any key to exit...";
	return 0;
}	

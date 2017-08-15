#include <GReflection.h>
#include <GSerializer.h>
#include "gserialization_test.h""
#include <sphere.h>

using namespace std;
using namespace GFramework;

void run_serialization_testcases()
{
	cout << endl << "Starting test cases for 'Serialization'..." << endl << endl;
	
	//Text serialization

	ofstream ots("test.txt");
	GTextSerializer g(ots);
	NodeSharedPtr np(NULL);
	sphere os1("s1", np, 1);

	NodeSharedPtr s1p(&os1);
	sphere os2("s2", s1p, 2);
	g << os1 << os2;
	ots.close();

	ifstream its("test.txt");
	GTextDeSerializer d(its);
	Object* is1 = nullptr;
	Object* is2 = nullptr;
	(d >> &is1) >> &is2;
	its.close();
	d.resolveDependencies();
}
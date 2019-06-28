#include <GReflection.h>
#include <GSerializer.h>
#include "gserialization_test.h""
#include <sphere.h>

using namespace std;
using namespace GFramework;

void run_serialization_testcases()
{
	cout << endl << "Starting test cases for 'Serialization'..." << endl << endl;
	
	NodeSharedPtr np(NULL);
	auto os1 = new sphere("s1", np, 1);
	NodeSharedPtr s1p(os1);
	sphere os2("s2", s1p, 2);

	{
		//Text serialization
		GTextSerializer ts;
		ts.open("test.txt");
		ts << os1 << os2;
		ts.close();

		//Text serialization
		GTextDeSerializer tds;
		tds.open("test.txt");
		Object* is1 = nullptr;
		Object* is2 = nullptr;
		(tds >> &is1) >> &is2;
		tds.close();
		tds.resolveDependencies();
	}

	{
		//Binary serialization
		GBinarySerializer bs;
		bs.open("test.bin");
		bs << os1 << os2;
		bs.close();

		//Binary serialization
		GBinaryDeSerializer bds;
		bds.open("test.bin");
		Object* is1 = nullptr;
		Object* is2 = nullptr;
		(bds >> &is1) >> &is2;
		bds.close();
		bds.resolveDependencies();
	}

	s1p.reset();
}
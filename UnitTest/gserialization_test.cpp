#include <GFramework/GReflection/GReflection.h>
#include <GFramework/GSerialization/GSerializer.h>
#include "gserialization_test.h""
#include <Node.h>
#include <Sphere.h>

using namespace std;
using namespace GFramework;
using namespace GFrameworkTest;

void run_serialization_testcases()
{
	cout << endl << "Starting test cases for 'Serialization'..." << endl << endl;
	
	NodeSharedPtr np(NULL);
	auto os1 = new sphere("s1", np, 25);
	NodeSharedPtr s1p(os1);
	sphere os2("s2", s1p, 15);

	{
		//Text serialization
		GTextSerializer ts;
		ts.open("test.txt");
		ts << os1 << os2;
		ts.close();

		//Text serialization
		GTextDeSerializer tds;
		tds.open("test.txt");
		GObject* is1 = nullptr;
		GObject* is2 = nullptr;
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
		GObject* is1 = nullptr;
		GObject* is2 = nullptr;
		(bds >> &is1) >> &is2;
		bds.close();
		bds.resolveDependencies();
	}

	s1p.reset();
}
#include <cassert>
#include <iostream>
#include <GVariant/GVariant.h>
#include <GReflection/GReflection.h>
#include <Node.h>
#include <Sphere.h>
#include <boost/optional.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
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

void run_reflection_testcases()
{
	cout << "=============================" << endl;
	cout << "Startig GReflection testcases" << endl;
	cout << "=============================" << endl;


	GMetaclass *objectmeta = GMetaclassList::instance().getMetaclass("GObject");
	GMetaclass *nodemeta = GMetaclassList::instance().getMetaclass("node");
	GMetaclass *spheremeta = GMetaclassList::instance().getMetaclass("sphere");

	NodeSharedPtr np(NULL);
	NodeSharedPtr parent = make_shared<Node>("GrandParentNode", np);
	Node n("ParentNode", parent);
	SphereSharedPtr sphere_parent = make_shared<sphere>("SphereParentNode", np, 321);
	sphere s("SphereNode", parent);
	
	std::vector<std::string> f_list;

	cout << "\nSphere class constructors:" << endl;
	{
		GMetaclassList::instance().getMetaclassByType<sphere>()->getConstructorsList(f_list);
		for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
			cout << *it << ": " << GMetaclassList::instance().getMetaclassByType<sphere>()->getConstructor((*it).c_str())->getPrototype() << endl;
		}
		f_list.clear();
	}

	cout << "\nSphere class default constructors invoke:" << endl;
	{
		GMetaconstructor* cons1 = GMetaclassList::instance().getMetaclassByType<sphere>()->getConstructor("DefaultCons");
		std::vector<GVariant> args;
		GObject* sphere1 = cons1->invoke(args);
		sphere1->about();
	}

	cout << "\nObject class static functions:" << endl;
	{
		GMetaclassList::instance().getMetaclassByType<GObject>()->getStaticFunctionsList(f_list);
		for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
			cout << GMetaclassList::instance().getMetaclassByType<GObject>()->getStaticFunction((*it).c_str())->getReturntype();
			cout << " " << *it << " ";
			cout << GMetaclassList::instance().getMetaclassByType<GObject>()->getStaticFunction((*it).c_str())->getPrototype() << endl;
		}
		f_list.clear();
	}

	cout << "\nObject class member functions:" << endl;
	GMetaclassList::instance().getMetaclassByType<GObject>()->getFunctionsList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nObject class properties:" << endl;
	GMetaclassList::instance().getMetaclassByType<GObject>()->getEditablePropertiesList(f_list);
	GMetaclassList::instance().getMetaclassByType<GObject>()->getPropertiesList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nNode class static member functions:" << endl;
	GMetaclassList::instance().getMetaclassByType<Node>()->getStaticFunctionsList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nNode class member functions:" << endl;
	GMetaclassList::instance().getMetaclassByType<Node>()->getFunctionsList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nNode class properties:" << endl;
	GMetaclassList::instance().getMetaclassByType<Node>()->getEditablePropertiesList(f_list);
	GMetaclassList::instance().getMetaclassByType<Node>()->getPropertiesList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nSphere class static member functions:" << endl;
	GMetaclassList::instance().getMetaclassByType<sphere>()->getStaticFunctionsList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nSphere class member functions:" << endl;
	GMetaclassList::instance().getMetaclassByType<sphere>()->getFunctionsList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nSphere class properties:" << endl;
	GMetaclassList::instance().getMetaclassByType<sphere>()->getEditablePropertiesList(f_list);
	GMetaclassList::instance().getMetaclassByType<sphere>()->getPropertiesList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();


	cout << "\nInvoke static function returning void and taking void arguments:" << endl;
	{
		auto sf = objectmeta->getStaticFunction("about");
		vector<GVariant> args;
		GVariant r = sf->invoke(args);
		assert(r.empty());
	}

	cout << "\nInvoke static function returning non-void and taking void arguments:" << endl;
	{
		auto sf = objectmeta->getStaticFunction("count");
		vector<GVariant> args;
		GVariant r = sf->invoke(args);
		assert(GVariant::cast<unsigned int>(r)==112233);
	}

	cout << "\nInvoke static function returning non-void and taking non-void arguments:" << endl;
	{
		auto sf = objectmeta->getStaticFunction("add");
		vector<GVariant> args;
		args.push_back((int)23);
		args.push_back((int)27);
		GVariant r = sf->invoke(args);
		assert(GVariant::cast<int>(r) == 50);
	}

	cout << "\nInvoke static function returning void and taking non-void arguments:" << endl;
	{
		auto sf = objectmeta->getStaticFunction("updateMagicNumber");
		vector<GVariant> args;
		args.push_back(23);
		GVariant r = sf->invoke(args);
	}

	cout << "\nInvoke member function returning void and taking constant reference arguments:" << endl;
	{
		auto f = objectmeta->getMemberFunction("rename");
		vector<GVariant> args;
		string name = "ParentNode-Renamed";
		const string& re = name;
		GVariant g;
		g = GVariant::create<const string&>(re);
		args.push_back(g);
		GVariant r = f->invoke(&n, args);
	}

	cout << "\nInvoke member function returning constant reference and taking void arguments:" << endl;
	{
		auto f1 = objectmeta->getMemberFunction("getObjectId");
		vector<GVariant> args_f1;
		GVariant r = f1->invoke(&n, args_f1);

		auto f2 = objectmeta->getMemberFunction("setObjectId");
		vector<GVariant> args_f2;
		args_f2.push_back(444U);
		f2->invoke(&n, args_f2);

		assert(GVariant::cast<const unsigned int&>(r) == 444);
	}

	// set a property value which invokes the onupdate notification function
	cout << "\nSet a property value which invokes the onupdate notification function:" << endl;
	{
		GMetaproperty *m = spheremeta->getProperty("name");
		GVariant v = m->get(&s);
		cout << "value of " << m->getName() << " is " << GVariant::cast<string>(v) << endl;
		v = string("new_sphere1");
		m->set(&s, v);
		cout << "value of " << m->getName() << " is " << GVariant::cast<string>(v) << endl;
	}

	// call function taking void arguments and return void 
	cout << "\nCall member function taking void arguments and return void :" << endl;
	{
		auto m = nodemeta->getMemberFunction("about");
		std::vector<GVariant> args;
		GVariant v = m->invoke(&n, args);
	}

	// call function taking void arguments but return a value
	cout << "\nCall member function taking void arguments but return a value:" << endl;
	{
		auto m = spheremeta->getMemberFunction("getRadius");
		std::vector<GVariant> args;
		GVariant rv = m->invoke(&s, args);
		unsigned int r = GVariant::cast<unsigned int>(rv);
		assert(r == 1);
	}

	// call function taking non-void arguments and return void
	cout << "\nCall member function taking non-void arguments and return void:" << endl;
	{
		auto m1 = spheremeta->getMemberFunction("setRadius");
		std::vector<GVariant> args1;
		args1.push_back(7u);
		GVariant r = m1->invoke(&s, args1);

		auto m2 = spheremeta->getMemberFunction("getRadius");
		std::vector<GVariant> args2;
		GVariant rv = m2->invoke(&s, args2);
		assert(GVariant::cast<unsigned int>(rv) == 7u);
	}

	// call function taking void arguments but return a shared pointer
	cout << "\nCall function taking void arguments but return a shared pointer:" << endl;
	{
		auto* m = spheremeta->getMemberFunction("getParent");
		std::vector<GVariant> args;
		GVariant rv = m->invoke(&s, args);
		Node* r = GVariant::cast<Node*>(rv);
		assert(r->getName() == "GrandParentNode");
	}

	// call function taking shared pointer arguments but return void
	cout << "\nCall member function taking shared pointer arguments but return void:" << endl;
	{
		auto m1 = spheremeta->getMemberFunction("setParent");
		std::vector<GVariant> args1;
		args1.push_back(static_pointer_cast<Node>(sphere_parent));
//		args1.push_back(sphere_parent);
		GVariant rv1 = m1->invoke(&s, args1);

		auto m2 = spheremeta->getMemberFunction("getParent");
		std::vector<GVariant> args2;
		GVariant rv2 = m2->invoke(&s, args2);
		Node* r2 = GVariant::cast<Node*>(rv2);
		assert(r2->getObjectId() == sphere_parent->getObjectId());
	}
	
	//call a function of base class
	cout << "\nCall a non-virtual member function of base class:" << endl;
	{
		auto m = spheremeta->getMemberFunction("setPosition");
		std::vector<GVariant> args;
		args.push_back(1.0f);
		args.push_back(2.0f);
		args.push_back(3.0f);
		GVariant r = m->invoke(&s, args);
		glm::vec3 p = s.getPosition();
		assert(p.x == 1.0f);
		assert(p.y == 2.0f);
		assert(p.z == 3.0f);
	}

	cout << "\nRead member property of type unsigned int:" << endl;
	{
		GMetaproperty *m = spheremeta->getProperty("radius");
		GVariant v = m->get(&s);
		assert(GVariant::cast<unsigned int>(v) == 7);
		cout << "value of " << m->getName() << " is " << GVariant::cast<unsigned int>(v) << endl;
	}

	cout << "\nModify member property of type unsigned int:" << endl;
	{
		GMetaproperty *m = spheremeta->getProperty("radius");
		GVariant v = 12U;
		m->set(&s, v);
		assert(s.getRadius() == 12U);
		cout << "modified the value of " << m->getName() << " to " << GVariant::cast<unsigned int>(v) << endl;
	}
}


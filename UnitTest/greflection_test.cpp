#include <cassert>
#include <iostream>

#include <gtest/gtest.h>

#include <GFramework/GVariant/GVariant.h>
#include <GFramework/GReflection/GReflection.h>
#include <Node.h>
#include <Sphere.h>
#ifndef Q_MOC_RUN
#include <boost/optional.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#endif
#include "greflection_test.h"

using namespace std;
using namespace GFramework;
using namespace GFrameworkTest;
/*	Requirement:
	> get all the reflected variables and functions of a class along with the data types.
	> need an easy way to make the class to register the metadata only once
*/

/*	Notes:
> 
*/

class GReflectionTest : public ::testing::Test {
protected:
	void SetUp() override {

		auto GFrameworkTest = GMetaNamespaceList::_global()._namespace("GFrameworkTest");
		auto GFramework = GMetaNamespaceList::_global()._namespace("GFramework");
		objectmeta = GFramework.getMetaclass("GObject");// GMetaclassList::instance().getMetaclass("GObject");
		nodemeta = GFrameworkTest.getMetaclass("Node");// GMetaclassList::instance().getMetaclass("node");
		spheremeta = GFrameworkTest.getMetaclass("sphere");// GMetaclassList::instance().getMetaclass("sphere");
		node_parent = NodeSharedPtr(NULL);
		parent = make_shared<Node>("GrandParentNode", node_parent);
		node1 = Node("ParentNode", parent);
		sphere1_parent = make_shared<sphere>("SphereParentNode", node_parent, 321);
		sphere1 = sphere("SphereNode", parent);
	}
protected:
	GMetaclass* objectmeta;
	GMetaclass* nodemeta;
	GMetaclass* spheremeta;
	NodeSharedPtr node_parent;
	NodeSharedPtr parent;
	Node node1;
	SphereSharedPtr sphere1_parent;
	sphere sphere1;
	std::vector<std::string> f_list;
};
namespace GFrameworkTest
{
	extern uint32 globalCounter;
}

#if 1 
TEST_F(GReflectionTest, CallNamespaceFuntion)
{
	GMetaNamespace nm = GMetaNamespaceList::_global()._namespace("GFrameworkTest");
	uint32 prev_counter = globalCounter;
	GMetafunction* fn = nm.getMetaFunction("IncrementGlobalCounter");
	EXPECT_NE(fn, nullptr);
	std::vector<GVariant> args;
	fn->invoke(args);
	EXPECT_EQ(prev_counter + 1, globalCounter);
}


TEST_F(GReflectionTest, CallNestedNamespaceFuntion)
{
	GMetaNamespace nm1 = GMetaNamespaceList::_global()._namespace("GFrameworkTest");
	GMetaNamespace nm2 = nm1._namespace("sphere_func");
	GMetafunction* fn = nm2.getMetaFunction("getSphereVolume");
	EXPECT_NE(fn, nullptr);
	std::vector<GVariant> args;
	GVariant sv = GVariant::ref<sphere>(sphere1);
	args.push_back(sv);
	GVariant r = fn->invoke(args);
	EXPECT_EQ(GVariant::cast<float>(r), (4.0f / 3.0f * 3.14f));
}

TEST_F(GReflectionTest, ListConstructors)
{
	GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere")->getConstructorsList(f_list);
	EXPECT_EQ(f_list.size(), 2);

	/*for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		std::cout << *it << ": " << GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere")->getConstructor((*it).c_str())->getPrototype() << endl;
	}
	f_list.clear();*/
}

TEST_F(GReflectionTest, CallDefaultConstructor)
{
	GMetaconstructor* cons1 = GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere")->getConstructor("DefaultCons");
	std::vector<GVariant> args;
	sphere* sphere1 = cons1->invoke<sphere>(args);
	EXPECT_NE(sphere1, nullptr);
	EXPECT_EQ(sphere1->getRadius(), 123);
}

TEST_F(GReflectionTest, CallNonDefaultConstructor)
{
	GMetaconstructor* cons1 = GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere")->getConstructor("Cons1");
	std::vector<GVariant> args;
	args.push_back(GVariant::create<unsigned int>(73));
	sphere* sphere1 = cons1->invoke<sphere>(args);
	EXPECT_NE(sphere1, nullptr);
	auto* sphere2 = cons1->invoke(args);
	EXPECT_NE(sphere2, nullptr);
	EXPECT_EQ(sphere1->getRadius(), 73);
}

TEST_F(GReflectionTest, ListClassStaticFunctions)
{
	GMetaNamespaceList::_global()._namespace("GFramework").getMetaclass("GObject")->getStaticFunctionsList(f_list);
	EXPECT_EQ(f_list.size(), 1);
	/*for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << GMetaNamespaceList::_global()._namespace("GFramework").getMetaclass("GObject")->getStaticFunction((*it).c_str())->getReturntype();
		cout << " " << *it << " ";
		cout << GMetaNamespaceList::_global()._namespace("GFramework").getMetaclass("GObject")->getStaticFunction((*it).c_str())->getPrototype() << endl;
	}
	f_list.clear();*/
}

TEST_F(GReflectionTest, ListBaseClassPublicMemberFunctions)
{
	GMetaNamespaceList::_global()._namespace("GFramework").getMetaclass("GObject")->getFunctionsList(f_list);
	EXPECT_EQ(f_list.size(), 3);
	/*for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();*/
}

TEST_F(GReflectionTest, DISABLED_ListDerivedClassPublicMemberFunctions)
{

}


TEST_F(GReflectionTest, InvokeStaticFunctionReturningVoidAndTakingVoidArguments)
{
	auto sf = objectmeta->getStaticFunction("about");
	vector<GVariant> args;
	GVariant r = sf->invoke(args);
	EXPECT_EQ(r.empty(), true);
}

//TODO: count is removed from GObject class for cleaning it, so add an alternate function in another class to test it
/*cout << "\nInvoke static function returning non-void and taking void arguments:" << endl;
{
	auto sf = objectmeta->getStaticFunction("count");
	vector<GVariant> args;
	GVariant r = sf->invoke(args);
	assert(GVariant::cast<unsigned int>(r) == 112233);
}*/

//TODO: add is removed from GObject class for cleaning it, so add an alternate function in another class to test it
/*cout << "\nInvoke static function returning non-void and taking non-void arguments:" << endl;
{
	auto sf = objectmeta->getStaticFunction("add");
	vector<GVariant> args;
	args.push_back((int)23);
	args.push_back((int)27);
	GVariant r = sf->invoke(args);
	assert(GVariant::cast<int>(r) == 50);
}*/

//TODO: updateMagicNumber is removed from GObject class for cleaning it, so add an alternate function in another class to test it
/*cout << "\nInvoke static function returning void and taking non-void arguments:" << endl;
{
	auto sf = objectmeta->getStaticFunction("updateMagicNumber");
	vector<GVariant> args;
	args.push_back(23);
	GVariant r = sf->invoke(args);
}*/


TEST_F(GReflectionTest, InvokeMemberFunctionReturningVoidAndTakingConstantReferenceArguments)
{
	auto f = objectmeta->getPublicMemberFunction("rename");
	vector<GVariant> args;
	string name = "ParentNode-Renamed";
	const string& re = name;
	GVariant g;
	g = GVariant::create<const string&>(re);
	args.push_back(g);
	GVariant r = f->invoke(&node1, args);
	EXPECT_EQ(r.empty(), true);
	EXPECT_EQ(node1.getName(),name);
}

TEST_F(GReflectionTest, InvokeMemberFunctionReturningConstantReferenceAndTakingVoidArguments)
{
	auto f1 = objectmeta->getPublicMemberFunction("getObjectId");
	vector<GVariant> argsF1;
	GVariant r = f1->invoke(&node1, argsF1);
	EXPECT_EQ(GVariant::cast<const unsigned int&>(r) > 0, true);
}

// call function taking void arguments and return void 
TEST_F(GReflectionTest, CallMemberFunctionTakingVoidArgumentsAndReturnVoid)
{
	auto m = nodemeta->getPublicMemberFunction("reset");
	std::vector<GVariant> args;
	node1.setPosition({ 5, 5, 5 });
	GVariant r = m->invoke(&node1, args);
	EXPECT_EQ(r.empty(), true);
	EXPECT_EQ(node1.getPosition(), glm::vec3(0,0,0));
}

// call function taking void arguments but return a value
TEST_F(GReflectionTest, CallMemberFunctionTakingVoidArgumentsButReturnValue)
{
	auto m = spheremeta->getPublicMemberFunction("getRadius");
	std::vector<GVariant> args;
	GVariant rv = m->invoke(&sphere1, args);
	unsigned int r = GVariant::cast<unsigned int>(rv);
	EXPECT_EQ(r, 1);
}

// call function taking non-void arguments and return void
TEST_F(GReflectionTest, CallMemberFunctionTakingNonvoidArgumentsAndReturnVoid)
{
	auto m1 = spheremeta->getPublicMemberFunction("setRadius");
	std::vector<GVariant> args1;
	args1.push_back(7u);
	GVariant r = m1->invoke(&sphere1, args1);

	auto m2 = spheremeta->getPublicMemberFunction("getRadius");
	std::vector<GVariant> args2;
	GVariant rv = m2->invoke(&sphere1, args2);
	EXPECT_EQ(GVariant::cast<unsigned int>(rv), 7u);
}

// call function taking void arguments but return a shared pointer
TEST_F(GReflectionTest, CallFunctionTakingVoidArgumentsButReturnSharedPointer)
{
	auto* m = spheremeta->getPublicMemberFunction("getParent");
	std::vector<GVariant> args;
	GVariant rv = m->invoke(&sphere1, args);
	Node* r = GVariant::cast<Node*>(rv);
	EXPECT_EQ(r->getName(), "GrandParentNode");
}

// call function taking shared pointer arguments but return void
TEST_F(GReflectionTest, CallMemberFunctionTakingSharedPointerArgumentsButReturnVoid)
{
	auto m1 = spheremeta->getPublicMemberFunction("setParent");
	std::vector<GVariant> args1;
	args1.push_back(static_pointer_cast<Node>(sphere1_parent));
	//		args1.push_back(sphereParent);
	GVariant rv1 = m1->invoke(&sphere1, args1);

	auto m2 = spheremeta->getPublicMemberFunction("getParent");
	std::vector<GVariant> args2;
	GVariant rv2 = m2->invoke(&sphere1, args2);
	Node* r2 = GVariant::cast<Node*>(rv2);
	EXPECT_EQ(r2->getObjectId(), sphere1_parent->getObjectId());
}

//call a function of base class
TEST_F(GReflectionTest, CallNonvirtualPublicMemberFunctionOfBaseClass)
{
	auto m = spheremeta->getPublicMemberFunction("setPosition");
	std::vector<GVariant> args;
	glm::vec3 pos(1.0f, 2.0f, 3.0f);
	args.push_back(GVariant::create<const glm::vec3&>(pos));
	GVariant r = m->invoke(&sphere1, args);
	glm::vec3 p = sphere1.getPosition();
	EXPECT_EQ(p, glm::vec3(1.0f, 2.0f, 3.0f));
}

//call a function of base class
TEST_F(GReflectionTest, CallVirtualPublicMemberFunctionUsingBaseClassPointerWithDerivedClassInstance)
{
	auto m1 = nodemeta->getPublicMemberFunction("reset");
	std::vector<GVariant> args;
	Node* np1 = new sphere(27);
	GVariant r = m1->invoke(np1, args);
	EXPECT_EQ(r.empty(), true);
	EXPECT_EQ(((sphere*)np1)->getRadius(), 0);
}

//TODO: see ifthis is possible
//cout << "\nForcefully call a virtual public member function of base class pointer with derived class instance:" << endl;
/*{

	auto m1 = nodemeta->getPublicMemberFunction("about");
	std::vector<GVariant> args;
	Node* np1 = new sphere(27);
	m1->invoke(np1, args); //Should invoke the Node::about() instead of sphere::about

}*/

TEST_F(GReflectionTest, ListThePropertiesOfNodeClass)
{
	std::vector<string> props;
	nodemeta->getPropertiesList(props);
	EXPECT_EQ(props.size(), 3);
	/*for (auto p : props)
	{
		cout << "Property: " << p << endl;
	}*/
}

TEST_F(GReflectionTest, SetPublicPropertyOfClass)
{
	auto pv = nodemeta->getProperty("visibility");
	node1.visibility = true;
	GVariant v = false;
	pv->set(&node1, v);
	EXPECT_EQ(node1.visibility.getValue(), false);
}

#else
void run_reflection_testcases()
{
	cout << "=============================" << endl;
	cout << "Startig GReflection testcases" << endl;
	cout << "=============================" << endl;

	GMetaNamespace GFrameworkTest = GMetaNamespaceList::_global()._namespace("GFrameworkTest");
	GMetaNamespace GFramework = GMetaNamespaceList::_global()._namespace("GFramework");
	GMetaclass *objectmeta = GFramework.getMetaclass("GObject");// GMetaclassList::instance().getMetaclass("GObject");
	GMetaclass *nodemeta = GFrameworkTest.getMetaclass("Node");// GMetaclassList::instance().getMetaclass("node");
	GMetaclass *spheremeta = GFrameworkTest.getMetaclass("sphere");// GMetaclassList::instance().getMetaclass("sphere");

	NodeSharedPtr np(NULL);
	NodeSharedPtr parent = make_shared<Node>("GrandParentNode", np);
	Node n("ParentNode", parent);
	SphereSharedPtr sphere_parent = make_shared<sphere>("SphereParentNode", np, 321);
	sphere s("SphereNode", parent);
	
	std::vector<std::string> f_list;

	cout << "\nPrint global namespace:" << endl;
	{
		GMetaNamespaceList::_global().print();
	}

	cout << "\nCall namespace funtion:" << endl;
	{
		GMetaNamespace nm = GMetaNamespaceList::_global()._namespace("GFrameworkTest");
		GMetafunction* fn = nm.getMetaFunction("NodeFileInfo");
		std::vector<GVariant> args;
		fn->invoke(args);
	}

	cout << "\nCall nested namespace funtion:" << endl;
	{
		GMetaNamespace nm1 = GMetaNamespaceList::_global()._namespace("GFrameworkTest");
		GMetaNamespace nm2 = nm1._namespace("sphere_func");
		GMetafunction* fn = nm2.getMetaFunction("getSphereVolume");
		std::vector<GVariant> args;
		GVariant sv = GVariant::ref<sphere>(s);
		args.push_back(sv);
		GVariant r = fn->invoke(args);
		assert(GVariant::cast<float>(r) == (4.0f / 3.0f*3.14f));
		cout << "Volume of sphere with radius " << s.getRadius() << " = " << GVariant::cast<float>(r) << endl;
	}

	cout << "\nList constructors:" << endl;
	{
		//GMetaNamespace::getMetaclassByType<sphere>()->getConstructorsList(f_list);
		GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere")->getConstructorsList(f_list);
		for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
			cout << *it << ": " << GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere")->getConstructor((*it).c_str())->getPrototype() << endl;
		}
		f_list.clear();
	}

	cout << "\nCall class default constructor:" << endl;
	{
		GMetaconstructor* cons1 = GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere")->getConstructor("DefaultCons");
		std::vector<GVariant> args;
		sphere* sphere1 = cons1->invoke<sphere>(args);
		sphere1->about();
	}

	cout << "\nClass non-default constructor:" << endl;
	{
		GMetaconstructor* cons1 = GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere")->getConstructor("Cons1");
		std::vector<GVariant> args;
		args.push_back(GVariant::create<unsigned int>(73));
		sphere* sphere1 = cons1->invoke<sphere>(args);
		auto* sphere2 = cons1->invoke(args);
		assert(sphere1->getRadius() == 73);
	}

	cout << "\nList class static functions:" << endl;
	{
		GMetaNamespaceList::_global()._namespace("GFramework").getMetaclass("GObject")->getStaticFunctionsList(f_list);
		for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
			cout << GMetaNamespaceList::_global()._namespace("GFramework").getMetaclass("GObject")->getStaticFunction((*it).c_str())->getReturntype();
			cout << " " << *it << " ";
			cout << GMetaNamespaceList::_global()._namespace("GFramework").getMetaclass("GObject")->getStaticFunction((*it).c_str())->getPrototype() << endl;
		}
		f_list.clear();
	}

	cout << "\nList base class public member functions:" << endl;
	{
		GMetaNamespaceList::_global()._namespace("GFramework").getMetaclass("GObject")->getFunctionsList(f_list);
		for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
			cout << *it << endl;
		}
		f_list.clear();
	}

	//cout << "\nList base class protected member functions:" << endl;
	{

	}

	//cout << "\nList derived class public member functions:" << endl;
	{

	}

	//cout << "\nList derived class protected member functions:" << endl;
	{

	}

	cout << "\nInvoke static function returning void and taking void arguments:" << endl;
	{
		auto sf = objectmeta->getStaticFunction("about");
		vector<GVariant> args;
		GVariant r = sf->invoke(args);
		assert(r.empty());
	}

	//TODO: count is removed from GObject class for cleaning it, so add an alternate function in another class to test it
	/*cout << "\nInvoke static function returning non-void and taking void arguments:" << endl;
	{
		auto sf = objectmeta->getStaticFunction("count");
		vector<GVariant> args;
		GVariant r = sf->invoke(args);
		assert(GVariant::cast<unsigned int>(r) == 112233);
	}*/

	//TODO: add is removed from GObject class for cleaning it, so add an alternate function in another class to test it
	/*cout << "\nInvoke static function returning non-void and taking non-void arguments:" << endl;
	{
		auto sf = objectmeta->getStaticFunction("add");
		vector<GVariant> args;
		args.push_back((int)23);
		args.push_back((int)27);
		GVariant r = sf->invoke(args);
		assert(GVariant::cast<int>(r) == 50);
	}*/

	//TODO: updateMagicNumber is removed from GObject class for cleaning it, so add an alternate function in another class to test it
	/*cout << "\nInvoke static function returning void and taking non-void arguments:" << endl;
	{
		auto sf = objectmeta->getStaticFunction("updateMagicNumber");
		vector<GVariant> args;
		args.push_back(23);
		GVariant r = sf->invoke(args);
	}*/

	cout << "\nInvoke member function returning void and taking constant reference arguments:" << endl;
	{
		auto f = objectmeta->getPublicMemberFunction("rename");
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
		auto f1 = objectmeta->getPublicMemberFunction("getObjectId");
		vector<GVariant> args_f1;
		GVariant r = f1->invoke(&n, args_f1);

		assert(GVariant::cast<const unsigned int&>(r) > 0);
	}

	// call function taking void arguments and return void 
	cout << "\nCall member function taking void arguments and return void :" << endl;
	{
		auto m = nodemeta->getPublicMemberFunction("about");
		std::vector<GVariant> args;
		GVariant v = m->invoke(&n, args);
	}

	// call function taking void arguments but return a value
	cout << "\nCall member function taking void arguments but return a value:" << endl;
	{
		auto m = spheremeta->getPublicMemberFunction("getRadius");
		std::vector<GVariant> args;
		GVariant rv = m->invoke(&s, args);
		unsigned int r = GVariant::cast<unsigned int>(rv);
		assert(r == 1);
	}

	// call function taking non-void arguments and return void
	cout << "\nCall member function taking non-void arguments and return void:" << endl;
	{
		auto m1 = spheremeta->getPublicMemberFunction("setRadius");
		std::vector<GVariant> args1;
		args1.push_back(7u);
		GVariant r = m1->invoke(&s, args1);

		auto m2 = spheremeta->getPublicMemberFunction("getRadius");
		std::vector<GVariant> args2;
		GVariant rv = m2->invoke(&s, args2);
		assert(GVariant::cast<unsigned int>(rv) == 7u);
	}

	// call function taking void arguments but return a shared pointer
	cout << "\nCall function taking void arguments but return a shared pointer:" << endl;
	{
		auto* m = spheremeta->getPublicMemberFunction("getParent");
		std::vector<GVariant> args;
		GVariant rv = m->invoke(&s, args);
		Node* r = GVariant::cast<Node*>(rv);
		assert(r->getName() == "GrandParentNode");
	}

	// call function taking shared pointer arguments but return void
	cout << "\nCall member function taking shared pointer arguments but return void:" << endl;
	{
		auto m1 = spheremeta->getPublicMemberFunction("setParent");
		std::vector<GVariant> args1;
		args1.push_back(static_pointer_cast<Node>(sphere_parent));
		//		args1.push_back(sphere_parent);
		GVariant rv1 = m1->invoke(&s, args1);

		auto m2 = spheremeta->getPublicMemberFunction("getParent");
		std::vector<GVariant> args2;
		GVariant rv2 = m2->invoke(&s, args2);
		Node* r2 = GVariant::cast<Node*>(rv2);
		assert(r2->getObjectId() == sphere_parent->getObjectId());
	}

	//call a function of base class
	cout << "\nCall a non-virtual public member function of base class:" << endl;
	{
		auto m = spheremeta->getPublicMemberFunction("setPosition");
		std::vector<GVariant> args;
		glm::vec3 pos(1.0f, 2.0f, 3.0f);
		args.push_back(GVariant::create<const glm::vec3&>(pos));
		GVariant r = m->invoke(&s, args);
		glm::vec3 p = s.getPosition();
		assert(p == glm::vec3(1.0f, 2.0f, 3.0f));
	}

	//call a function of base class
	cout << "\nCall a virtual public member function using base class pointer with derived class instance:" << endl;
	{
		auto m1 = nodemeta->getPublicMemberFunction("about");
		std::vector<GVariant> args;
		Node* np1 = new sphere(27);
		m1->invoke(np1, args);
	}

	//TODO: see ifthis is possible
	//cout << "\nForcefully call a virtual public member function of base class pointer with derived class instance:" << endl;
	{
		/*
		auto m1 = nodemeta->getPublicMemberFunction("about");
		std::vector<GVariant> args;
		Node* np1 = new sphere(27);
		m1->invoke(np1, args); //Should invoke the Node::about() instead of sphere::about
		*/
	}

	cout << "\nList the properties of a Node class" << endl;
	{
		std::vector<string> props;
		nodemeta->getPropertiesList(props);
		for (auto p : props)
		{
			cout << "Property: " << p << endl;
		}
	}

	cout << "\nSet the public property of a class" << endl;
	{
		auto pv = nodemeta->getProperty("visibility");
		n.visibility = true;
		GVariant v = false;
		pv->set(&n, v);
		assert(n.visibility.getValue() == false);
	}

#if 0
	cout << "\nNode class static member functions:" << endl;
	{
		GMetaNamespace::getMetaclassByType<Node>()->getStaticFunctionsList(f_list);
		for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
			cout << *it << endl;
		}
		f_list.clear();
	}

	cout << "\nNode class member functions:" << endl;
	GMetaNamespace::getMetaclassByType<Node>()->getFunctionsList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nNode class properties:" << endl;
	GMetaNamespace::getMetaclassByType<Node>()->getEditablePropertiesList(f_list);
	GMetaNamespace::getMetaclassByType<Node>()->getPropertiesList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nSphere class static member functions:" << endl;
	GMetaNamespace::getMetaclassByType<sphere>()->getStaticFunctionsList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nSphere class member functions:" << endl;
	GMetaNamespace::getMetaclassByType<sphere>()->getFunctionsList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

	cout << "\nSphere class properties:" << endl;
	GMetaNamespace::getMetaclassByType<sphere>()->getEditablePropertiesList(f_list);
	GMetaNamespace::getMetaclassByType<sphere>()->getPropertiesList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();




	cout << "\nObject class properties:" << endl;
	GFramework.getMetaclassByType<GObject>()->getEditablePropertiesList(f_list);
	GFramework.getMetaclassByType<GObject>()->getPropertiesList(f_list);
	for (auto it = f_list.cbegin(); it != f_list.cend(); ++it) {
		cout << *it << endl;
	}
	f_list.clear();

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
#endif
}
#endif

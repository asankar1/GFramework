#include <iostream>
#include "Sphere.h"

using namespace std;
using namespace GFrameworkTest;

	
BEGIN_DEFINE_META(sphere)
	GMetaNamespaceList::_global()._namespace("GFrameworkTest")
		.define<sphere>("sphere")
			.baseMetaclass("Node", {"GFrameworkTest"})
			.version(1)
			.constructor<void*()>("DefaultCons")
			.constructor<void*(unsigned int)>("Cons1")
			.functionPublic("setRadius", &sphere::setRadius)
			.functionPublic("getRadius", &sphere::getRadius)
			.editableProperty("radius", &sphere::radius);

	GMetaNamespaceList::_global()._namespace("GFrameworkTest")._namespace("sphere_func")
		.function("getSphereVolume", sphere_func::getSphereVolume);
END_DEFINE_META(sphere)
namespace GFrameworkTest
{

	namespace sphere_func
	{
		float getSphereVolume(sphere& s)
		{
			return 4.0f / 3.0f*3.14f*s.getRadius()*s.getRadius()*s.getRadius();
		}
	}

	sphere::sphere()
	{

	}

	sphere::sphere(unsigned int rad)
	{
		radius.setValue(rad);
	}

	sphere::sphere(const char *_name, NodeSharedPtr& _parent, unsigned int _radius) : Node(_name, _parent)
	{
		radius.setValue(_radius);
	}

	sphere::~sphere()
	{
	}

	void sphere::setRadius(unsigned int _radius)
	{
		radius.setValue(_radius);
		reconstruct();
	}

	unsigned int sphere::getRadius() const
	{
		return radius.getValue();
	}

	void sphere::about()
	{
		cout << "Type: " << "Sphere" << endl;
		cout << "Name: " << getName() << endl;
//temp		if (getParent() != NULL)
//temp			cout << "Parent: " << getParent()->getName() << endl;
		cout << "Position: (" << getPosition().x << "," << getPosition().y << "," << getPosition().z << ")" << endl;
		cout << "Radius: " << radius.getValue() << endl;
	}

	void sphere::reset()
	{
		radius = 0;
	}

	GMetaclass* sphere::getMetaclass() const
	{
		return GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("sphere");
	}

	void sphere::reconstruct()
	{

	}
}

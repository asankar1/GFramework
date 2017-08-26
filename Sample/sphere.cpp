#include <iostream>
#include "sphere.h"

using namespace std;

namespace GFramework
{
	BEGIN_DEFINE_META(sphere)
		GMetaclassList::instance().define<sphere>("sphere")
			.baseMetaclass("node")
			.function("setRadius", &sphere::setRadius)
			.function("getRadius", &sphere::getRadius)
			.editableProperty("radius", &sphere::radius);
	END_DEFINE_META(sphere)

	sphere::sphere(const char *_name, NodeSharedPtr& _parent, unsigned int _radius) : Node(_name, _parent)
	{
		radius.setValue(_radius);
		cout << "Sphere '" << getName() << "' constructed with radius " << radius.getValue() << endl;
	}

	sphere::~sphere()
	{
		cout << "Sphere '" << getName() << "' destroyed." << endl;
	}

	void sphere::setRadius(unsigned int _radius)
	{
		radius.setValue(_radius);
		cout << "Sphere '" << getName() << "' radius changed to " << radius.getValue() << endl;
		reconstruct();
	}

	unsigned int sphere::getRadius()
	{
		return radius.getValue();
	}

	void sphere::about()
	{
		cout << "Type: " << "Sphere" << endl;
		cout << "Name: " << getName() << endl;
		if (getParent() != NULL)
			cout << "Parent: " << getParent()->getName() << endl;
		cout << "Position: (" << getPosition().x << "," << getPosition().y << "," << getPosition().z << ")" << endl;
		cout << "Radius: " << radius.getValue() << endl;
	}

	const char * sphere::metaclassName()
	{
		return GMetaclassList::instance().getMetaclassByType<sphere>().getName().c_str();
	}

	void sphere::reconstruct()
	{
		cout << "Sphere node reconstructed!\n";
	}
}

#include <iostream>
#include "Object.h"
#include <Node.h>
using namespace std;

namespace GFramework
{
	//object id starts from 1. 0 is reservedfor null object
	std::atomic<unsigned int> Object::atomic_count = 1;
#if 1
	BEGIN_DEFINE_META(Object)
		GMetaclassList::instance().define<Object>("Object")
			.version(1)
			.function("getName", &Object::getName)
			.function("rename", &Object::rename)
			.function("getObjectId", &Object::getObjectId)
			.function("setObjectId", &Object::setObjectId)
			.staticFunction("about", Object::about)
			.function("rename", &Object::rename)
			.staticFunction("count", Object::count)	
			.staticFunction("add", Object::add)
			.staticFunction("updateMagicNumber", Object::updateMagicNumber)
			.editableProperty("name", &Object::name)
			.property("object_id", &Object::object_id);
	END_DEFINE_META(Object)
#endif
	Object::Object(const char *_name)
	{
		object_id.setValue(atomic_count.load());
		atomic_count++;
		name.setValue(string(_name));
		cout << "Object '" << name.getValue() << "' constructed." << endl;
	}

	Object::~Object()
	{
		cout << "Object '" << name.getValue() << "' destroyed." << endl;
	}

	void Object::rename(const std::string& _name)
	{
		cout << "Old name '" << name.getValue() << "' renamed to new name '" << _name << "'!" << endl;
		name.setValue(_name);
	}

	const std::string & Object::getName() const
	{
		return name.getValue();
	}

	const unsigned int & Object::getObjectId() const
	{
		return object_id.getValue();
	}

	void Object::setObjectId(unsigned int _newid)
	{
		object_id.setValue(_newid);
	}

	void Object::addObserver(Object* _object)
	{
		if (observers.find(_object) == observers.end())
		{
			observers[_object] = 1;
			cout << "Object: " << getName() << " adds a new observer: " << _object->getName() << endl;
		}
		else
		{
			observers[_object]++;
			cout << "Object: " << getName() << " increments the existing observer: " << _object->getName() << " count to " << observers[_object] << endl;
		}
	}

	void Object::removeObserver(Object* _object)
	{
		if (observers.find(_object) != observers.end())
		{
			--observers[_object];
			cout << "Object: " << getName() << " decrements the existing observer: " << _object->getName() << " count to " << observers[_object] << endl;
			if (observers[_object] == 0)
			{
				observers.erase(_object);
				cout << "Object: " << getName() << " removes the observer: " << _object->getName() << endl;
			}
		}
	}

	bool Object::serialize(GSerializer& serializer)
	{
		const char* metaclassname = metaclassName();
		GMetaclass* m = GMetaclassList::instance().getMetaclass(metaclassname);

		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		int i = 0;
		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());
			serializer.writeMetaProperty(this, p);
		}

		return true;
	}

	bool Object::deserialize(GDeserializer& deserializer, unsigned int version)
	{
		const char* metaclassname = metaclassName();
		GMetaclass* m = GMetaclassList::instance().getMetaclass(metaclassname);

		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		int i = 0;
		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());
			deserializer.readMetaProperty(this, p);
		}
		return true;
	}

}
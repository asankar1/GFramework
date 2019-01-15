#include <iostream>
#include <GVariant/GObject.h>
#include <GSerialization/GSerializer.h>
#include <GReflection/GReflection.h>
//#include <Node.h>
using namespace std;

namespace GFramework
{
	//object id starts from 1. 0 is reservedfor null object
	std::atomic<unsigned int> GObject::atomic_count(1);
#if 1
	BEGIN_DEFINE_META(GObject)
		GMetaclassList::instance().define<GObject>("GObject")
			.version(1)
			.function("getName", &GObject::getName)
			.function("rename", &GObject::rename)
			.function("getObjectId", &GObject::getObjectId)
			.function("setObjectId", &GObject::setObjectId)
			.staticFunction("about", GObject::about)
			.function("rename", &GObject::rename)
			.staticFunction("count", GObject::count)	
			.staticFunction("add", GObject::add)
			.staticFunction("updateMagicNumber", GObject::updateMagicNumber)
			.editableProperty("name", &GObject::name)
			.property("object_id", &GObject::object_id);
	END_DEFINE_META(GObject)
#endif
	GObject::GObject(const char *_name)
	{
		object_id.setValue(atomic_count.load());
		atomic_count++;
		name.setValue(string(_name));
		cout << "GObject '" << name.getValue() << "' constructed." << endl;
	}

	GObject::~GObject()
	{
		cout << "GObject '" << name.getValue() << "' destroyed." << endl;
	}

	void GObject::rename(const std::string& _name)
	{
		cout << "Old name '" << name.getValue() << "' renamed to new name '" << _name << "'!" << endl;
		name.setValue(_name);
	}

	const std::string & GObject::getName() const
	{
		return name.getValue();
	}

	const unsigned int & GObject::getObjectId() const
	{
		return object_id.getValue();
	}

	void GObject::setObjectId(unsigned int _newid)
	{
		object_id.setValue(_newid);
	}

	void GObject::addObserver(GObject* _object)
	{
		if (observers.find(_object) == observers.end())
		{
			observers[_object] = 1;
			cout << "GObject: " << getName() << " adds a new observer: " << _object->getName() << endl;
		}
		else
		{
			observers[_object]++;
			cout << "GObject: " << getName() << " increments the existing observer: " << _object->getName() << " count to " << observers[_object] << endl;
		}
	}

	void GObject::removeObserver(GObject* _object)
	{
		if (observers.find(_object) != observers.end())
		{
			--observers[_object];
			cout << "GObject: " << getName() << " decrements the existing observer: " << _object->getName() << " count to " << observers[_object] << endl;
			if (observers[_object] == 0)
			{
				observers.erase(_object);
				cout << "GObject: " << getName() << " removes the observer: " << _object->getName() << endl;
			}
		}
	}

	bool GObject::serialize(GSerializer& serializer)
	{
#if 1
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
#endif
		return true;
	}

	bool GObject::deserialize(GDeserializer& deserializer, unsigned int version)
	{
#if 1
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
#endif
		return true;
	}

}

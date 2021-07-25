#include <iostream>
#include <GFramework/GVariant/GObject.h>
#include <GFramework/GSerialization/GSerializer.h>
#include <GFramework/GReflection/GReflection.h>
//#include <Node.h>
using namespace std;

namespace GFramework
{
	//object id starts from 1. 0 is reservedfor null object
	std::atomic<unsigned int> GObject::atomic_count(1);

	/*BEGIN_DEFINE_META(GObject)
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
	END_DEFINE_META(GObject)*/

	BEGIN_DEFINE_META(GObject)
		GMetaNamespaceList::_global()._namespace("GFramework")
		.define<GObject>("GObject")
		.version(1)
		.functionPublic("getName", &GObject::getName)
		.functionPublic("rename", &GObject::rename)
		.functionPublic("getObjectId", &GObject::getObjectId)
		.staticFunction("about", GObject::about)
		.functionPublic("rename", &GObject::rename)
		.editableProperty("name", &GObject::getName, &GObject::rename);
	END_DEFINE_META(GObject)

	GObject::GObject(const char *_name)
	{
		object_id.setValue(atomic_count.load());
		atomic_count+=1;
		name.setValue(string(_name));
	}
	
	GObject::GObject() : GObject("unnamed")
	{

	}

	GObject::GObject(const GObject& obj)
	{
		*this = obj;
	}

	GObject::GObject(const GObject&& obj)
	{
		*this = obj;
	}

	GObject& GObject::operator=(const GObject& obj)
	{
		name = obj.name;
		object_id.setValue(atomic_count.load());
		atomic_count++;
		observers = obj.observers;
		deletion_subscribers = obj.deletion_subscribers;
		return *this;
	}

	GObject& GObject::operator=(const GObject&& obj)
	{
		name = obj.name;
		object_id.setValue(atomic_count.load());
		atomic_count++;
		observers = obj.observers;
		deletion_subscribers = obj.deletion_subscribers;
		return *this;
	}

	GObject::~GObject()
	{
		for_each(deletion_subscribers.begin(), deletion_subscribers.end(), [](GPointerPropertyInterface* p) {p->subjectDeleted(); });
	}

	void GObject::rename(const std::string& _name)
	{
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

	void GObject::subscribeDeletionNotification(GPointerPropertyInterface* _subscriber)
	{
		deletion_subscribers.insert(_subscriber);
	}

	void GObject::unSubscribeDeletionNotification(GPointerPropertyInterface* _subscriber)
	{
		deletion_subscribers.erase(_subscriber);
	}

	bool GObject::serialize(GSerializer& serializer)
	{
		//const char* metaclassname = metaclassName();
		GMetaclass* m = getMetaclass();

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

	bool GObject::deserialize(GDeserializer& deserializer, unsigned int version)
	{
		//const char* metaclassname = metaclassName();
		GMetaclass* m = getMetaclass();

		std::vector<std::string> p_list;
		m->getEditablePropertiesList(p_list);
		m->getPropertiesList(p_list);

		int i = 0;
		for (auto it = p_list.cbegin(); it != p_list.cend(); ++it) {
			string property_name = *it;
			auto p = m->getProperty(property_name.c_str());
			deserializer.readMetaProperty(shared_from_this(), p);
		}
		return true;
	}

}

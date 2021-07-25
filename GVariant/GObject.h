#pragma once
#include <iostream>
#include <atomic>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <GFramework/GVariant/GProperty.h>
#include <GFramework/GReflection/GReflection.h>
//#include <GFramework/GSerialization/GSerializer.h>

/*! \file GObject.h
*	\brief class, functions, enums, typedefs, macros and other definitions related to GObject class.
*/

namespace GFramework
{
	class GObject;
	class GSerializer;
	class GDeserializer;
	class GPointerPropertyInterface;

	/** \var typedef std::shared_ptr<GObject> GObjectSharedPtr;
	*	\brief shared pointer type for the Node class
	*/
	typedef std::shared_ptr<GObject> GObjectSharedPtr;

	/*! \brief This the base class for all the other classes.
	*/
	class GFRAMEWORK_API GObject : public std::enable_shared_from_this<GObject>
	{
	public:
		GObject(const GObject& obj);

		GObject(const GObject&& obj);

		GObject& operator=(const GObject&);

		GObject& operator=(const GObject&&);

		virtual ~GObject();

		/**
		* Change the name of the existing object
		* \param _name is a string reference of the new name.
		* \return void
		*/
		void rename(const std::string& _name);

		/**
		* Get the id of the object
		* \return object_id as const unsigned int
		*/
		const unsigned int& getObjectId() const;

		/**
		* Add a new object to the observers list
		* \return void
		*/
		void addObserver(GObject* _object);

		/**
		* Remove an object from the observers list
		* \return void
		*/
		void removeObserver(GObject* _object);

		/**
		* This is a pure virtual function, all the inheritors should implement this.
		* Typically this can be used to do any required initialization for the obejct or do nothing.
		* \return void
		*/
		virtual void initialize() = 0;

		/**
		* This is a pure virtual function, all the inheritors should implement this.
		* Typically this should return the metaclass of the class.
		* \return constant pointer to the string
		*/
		virtual GMetaclass* getMetaclass() const = 0;

		/**
		* This is a virtual function, all the inheritors may override to have its own implementation.
		* The default implementation will use the meta class info to serialize all the properties.
		* Any implementation should ensure that all the base classes are serialized before its own properties.
		* \param serializer is the GSerializer using which the properties must be serialized.
		* \return the result of serialization.
		*/
		virtual bool serialize(GSerializer& serializer);

		/**
		* This is a virtual function, all the inheritors may override to have its own implementation.
		* The default implementation will use the meta class info to deserialize all the properties.
		* Any implementation should ensure that all the base classes are deserialized before its own properties.
		* \param stream is the output stream to which the properties must be serialized.
		* \param version is the version of the class saved into the stream. Use this to handle files saved with previous class version.
		* \return result of the deserialization
		*/
		virtual bool deserialize(GDeserializer& deserializer, unsigned int version);

		void subscribeDeletionNotification(GPointerPropertyInterface* _observer);

		void unSubscribeDeletionNotification(GPointerPropertyInterface* _observer);

		static void about() {

		}

		/**
		* Get the name of the object
		* \return name as const string reference
		*/
		const std::string & getName() const;

	protected:

		/**
		* Constructs the GObject class.
		* \param _name is a string reference.
		* \return Nil
		*/
		GObject(const char *_name);

		GObject();

	private:
		/**
		* set the id of the object
		* \param _newid is an unsigned int as the new object_id
		* \return void
		*/
		void setObjectId(uint32 _newid);

		GStringProperty name; /*!< Name of the object*/
		GUint32Property object_id;
		static std::atomic<unsigned int> atomic_count;
		std::map<GObject*,  unsigned int> observers;
		std::set<GPointerPropertyInterface*> deletion_subscribers;
		friend GDeserializer;
		//DECLARE_META_FRIEND(GObject);
	};
}
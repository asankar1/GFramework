#pragma once
#include <iostream>
#include <atomic>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <GVariant/GProperty.h>
#include <GReflection/GReflection.h>
//#include <GSerialization/GSerializer.h>

#ifdef VARIANT_DYNAMIC_LIBRARY
#ifdef DLL_EXPORT
#define LIBRARY_API __declspec( dllexport )
#else
#define LIBRARY_API __declspec( dllimport )
#endif
#else
#define LIBRARY_API
#endif

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
	class GFRAMEWORK_API GObject
	{
	public:
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
		* set the id of the object
		* \param _newid is an unsigned int as the new object_id
		* \return void
		*/
		void setObjectId(unsigned int _newid);

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
		* Typically this should return the metaclass name of the class.
		* \return constant pointer to the string
		*/
		virtual GMetaclass* metaclassName() = 0;

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

		void name_modified() {
			std::cout << "someone modified my name" << std::endl;
		}

		static void about() {
			std::cout << "This is the base object!" << std::endl;
		}

		static unsigned int count() {
			return 112233;
		}

		static int add(int x, int y) {
			return (x+y);
		}

		static void updateMagicNumber(int x) {
			static int i;
			std::cout << "Old MagicNumber is " << i << " and new MagicNumber is " << x << std::endl;
			i = x;
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

		GObject() {}

	private:
		GStringProperty name; /*!< Name of the object*/
		GUint32Property object_id;
		static std::atomic<unsigned int> atomic_count;
		std::map<GObject*,  unsigned int> observers;
		std::set<GPointerPropertyInterface*> deletion_subscribers;
		//META_FRIEND(GObject);
	};
}
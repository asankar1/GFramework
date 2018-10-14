#pragma once
#include <iostream>
#include <atomic>
#include <string>
#include <memory>
#include <property.h>
#include <GReflection.h>
#ifdef VARIANT_DYNAMIC_LIBRARY
#ifdef DLL_EXPORT
#define LIBRARY_API __declspec( dllexport )
#else
#define LIBRARY_API __declspec( dllimport )
#endif
#else
#define LIBRARY_API
#endif

/*! \file Object.h
*	\brief class, functions, enums, typedefs, macros and other definitions related to Object class.
*/

namespace GFramework
{
	class Object;

	/** \var typedef std::shared_ptr<Object> ObjectSharedPtr;
	*	\brief shared pointer type for the Node class
	*/
	typedef std::shared_ptr<Object> ObjectSharedPtr;

	/*! \brief This the base class for all the other classes.
	*/
	class LIBRARY_API Object
	{
	public:
		~Object();

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
		void addObserver(Object* _object);

		/**
		* Remove an object from the observers list
		* \return void
		*/
		void removeObserver(Object* _object);

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
		virtual const char* metaclassName() = 0;

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



	protected:
		/**
		* Get the name of the object
		* \return name as const string reference
		*/
		const std::string & getName() const;

		/**
		* Constructs the Object class.
		* \param _name is a string reference.
		* \return Nil
		*/
		Object(const char *_name);

		Object() {}

	private:
		GStringProperty name; /*!< Name of the object*/
		GUintProperty object_id;
		static std::atomic<unsigned int> atomic_count;
		std::map<Object*,  unsigned int> observers;
		META_FRIEND(Object);
	};
}
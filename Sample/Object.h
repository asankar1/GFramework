#pragma once
#include <atomic>
#include <string>
#include <memory>
#include <property.h>
#include <GVariant.h>
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
		* Get the name of the object
		* \return name as const string reference
		*/
		const std::string & getName() const;

		/**
		* Get the id of the object
		* \return object_id as const unsigned int
		*/
		const unsigned int& getObjectId() const;

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

		virtual const char* metaclassName() = 0;
	protected:
		/**
		* Constructs the Object class.
		* \param _name is a string reference.
		* \return Nil
		*/
		Object(const char *_name);

		Object() {}

	private:
		GStringProperty name; /*!< Name of the object*/
		GScalarProperty<unsigned int> object_id;
		static std::atomic<unsigned int> atomic_count;
		std::map<Object*,  unsigned int> observers;
		META_FRIEND(Object);
	};
}
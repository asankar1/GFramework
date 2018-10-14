#pragma once
#include "Node.h"

#ifdef VARIANT_DYNAMIC_LIBRARY
#ifdef DLL_EXPORT
#define LIBRARY_API __declspec( dllexport )
#else
#define LIBRARY_API __declspec( dllimport )
#endif
#else
#define LIBRARY_API
#endif

namespace GFramework
{
	class sphere;

	/** \var typedef std::shared_ptr<sphere> SphereSharedPtr;
	*	\brief shared pointer type for the Sphere class
	*/
	typedef std::shared_ptr<sphere> SphereSharedPtr;

	/*! \brief This the sphere class
	*/
	class LIBRARY_API sphere : public Node
	{
	public:
		/**
		* Constructs the sphere object.
		* \param _name is a string reference.
		* \param _parent is the parent object to the sphere onject
		* \param _radius is the radius of the sphere. Default value is 1.
		* \return Nil
		*/
		sphere(const char *_name, NodeSharedPtr& _parent, unsigned int _radius = 1);

		~sphere();

		/**
		* Sets the radius of the sphere object.
		* \param _radius is radius of the sphere
		* \return void
		*/
		void setRadius(unsigned int _radius);

		/**
		* Gets the radius of the sphere object.
		* \return radius of the sphere as const unsigned int.
		*/
		unsigned int getRadius() const;

		/**
		* Prints the information about the sphere
		* \return void
		*/
		virtual void about();

		virtual const char* metaclassName();
	protected:
		sphere() {}
		void reconstruct();

	private:
		GUintProperty radius; /*!< Radius of the sphere*/

		META_FRIEND(sphere);
	};
}


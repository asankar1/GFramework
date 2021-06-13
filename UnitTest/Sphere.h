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

DECLARE_META_CLASS(sphere);

namespace GFrameworkTest
{
	using namespace GFramework;
	class sphere;

	/** \var typedef std::shared_ptr<sphere> SphereSharedPtr;
	*	\brief shared pointer type for the Sphere class
	*/
	typedef std::shared_ptr<sphere> SphereSharedPtr;

	/*! \brief This the sphere class
	*/
	class sphere : public Node
	{
	public:
		sphere();

		sphere(unsigned int rad);
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

		virtual GMetaclass* getMetaclass() const;
	protected:
		//sphere() {}
		void reconstruct();

	private:
		GUint32Property radius; /*!< Radius of the sphere*/

		DECLARE_META_FRIEND(sphere);
	};

	namespace sphere_func
	{
		float getSphereVolume(sphere& s);
	}

}

#pragma once
#include <vector>
#include <glm\vec3.hpp>
#include <boost\shared_ptr.hpp>
#include <Object.h>
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

/*! \file Node.h
*	\brief class, functions, enums, typedefs, macros and other definitions related to Node class.
*/

namespace GFramework
{
	/*! \brief This the base class for all the types of nodes.
	*/
	class LIBRARY_API Node : public Object
	{
	public:
		/**
		* Constructs the node class.
		* \param _name is the name of the object.
		* \param _parent is the parent object to the sphere onject
		* \return Nil
		*/
		Node(const char *_name, NodeSharedPtr& _parent);

		~Node();

		/**
		* Sets the position of the node.
		* \param x is x position of the node
		* \param y is y position of the node
		* \param z is z position of the node
		* \return void
		*/
		void setPosition(float x, float y, float z);

		/**
		* Sets the position of the node.
		* \param pos is position of the node
		* \return void
		*/
		void setPosition_(glm::vec3 &pos);

		/**
		* Gets the position of the node.
		* \return position will be returned as const &glm::vec3 
		*/
		const glm::vec3& getPosition() const;

		/**
		* Prints the information about the node
		* \return void
		*/
		virtual void about();

		/**
		* Reassign the parent node
		* \param _parent as shared pointer for any Node object
		* \return void
		*/
		void setParent(NodeSharedPtr _parent);

		/**
		* get the parent of the node object		
		* \return parent node
		*/
		NodeSharedPtr getParent();

		/**
		* Add a child node to the node object
		* \param child as shared pointer for any Node object
		* \return void
		*/
		void addChild(NodeSharedPtr _child);

		/**
		* gets the child at the specified position
		* \param index as size_t
		* \return child at the index
		*/
		NodeSharedPtr getChild(size_t index);

		/**
		* Initializes the node object
		* \return void
		*/
		virtual void initialize();

		virtual const char* metaclassName();

	protected:
		Node() {}
	private:
		GGlmProperty<glm::vec3> position; /*!< Position of the node*/
		GNodePointerProperty<NodeSharedPtr> parent; /*!< Pointer to the parent of the node object*/
		std::vector<NodeSharedPtr> children;

		META_FRIEND(Node);
	};
}


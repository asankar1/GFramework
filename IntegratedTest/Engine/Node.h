#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <boost/shared_ptr.hpp>
#include <GFramework/GVariant/GObject.h>
#include <GFramework/GReflection/GReflection.h>

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

namespace GFrameworkTest
{
	using namespace GFramework;
	/*! \brief This the base class for all the types of nodes.
	*/

	void NodeFileInfo();

	class Node;
	typedef std::shared_ptr<Node> NodeSharedPtr;
    class GFRAMEWORKTEST_API Node : public GObject
	{
	public:
		/**
		* Constructs the node class.
		* \param _name is the name of the object.
		* \return Nil
		*/
		Node(const char *_name);

		/**
		* Constructs the node class as a child to a parent node.
		* \param _name is the name of the object.
		* \param _parent is the parent object to the sphere onject
		* \return Nil
		*/
		Node(const char *_name, NodeSharedPtr& _parent);

		~Node();

		/**
		* Sets the position of the node.
		* \param _pos is position of the node
		* \return void
		*/
		void setPosition(const glm::vec3& _pos);

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
        NodeSharedPtr getParent() const;

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
        NodeSharedPtr getChild(size_t index) const;

        /**
        * gets the child count
        * \return child count
        */
        size_t getChildCount() const;

		/**
		* Initializes the node object
		* \return void
		*/
		virtual void initialize();

        //virtual GMetaclass* metaclassName();

        virtual GMetaclass* getMetaclass() const override;

		//protected://TODO: Fix
		Node():GObject() {}

        GBoolProperty visibility;
        GVec3Property position; /*!< Position of the node*/
	protected:
		const char* getMagicString();

	private:

		GPointerProperty<Node> parent; /*!< Pointer to the parent of the node object*/
		std::vector<NodeSharedPtr> children;

		//META_FRIEND(Node);
	};
}


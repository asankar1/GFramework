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

DECLARE_META_CLASS(Node);

namespace GFrameworkTest
{
	using namespace GFramework;
	/*! \brief This the base class for all the types of nodes.
	*/

	void NodeFileInfo();

	class Node;
	typedef std::shared_ptr<Node> NodeSharedPtr;
	class GFRAMEWORK_TEST_API Node : public GObject
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
		* Reset the position to default
		* \return void
		*/
		virtual void reset();

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
		Node* getParent();

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

		virtual GMetaclass* getMetaclass() const;

		//protected://TODO: Fix
		Node():GObject() {}

		GBoolProperty visibility;
	protected:
		const char* getMagicString();

	private:
		GVec3Property position; /*!< Position of the node*/
		GPointerProperty<Node> parent; /*!< Pointer to the parent of the node object*/
		std::vector<NodeSharedPtr> children;

		DECLARE_META_FRIEND(Node);
	};
}


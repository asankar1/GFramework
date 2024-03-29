#include <iostream>
#include "Node.h"
using namespace std;
using namespace GFramework;
using namespace GFrameworkTest;

//template class __declspec(dllexport) GMetaNonAbstractclass<GFrameworkTest::Node>;
namespace GFrameworkTest
{
	uint32 globalCounter = 0;
	void IncrementGlobalCounter();
}

BEGIN_DEFINE_META(Node)
	GMetaNamespaceList::_global()._namespace("GFrameworkTest")
	.function("IncrementGlobalCounter", GFrameworkTest::IncrementGlobalCounter)
	.define<Node>("Node")
	.baseMetaclass("GObject", { "GFramework" })
	.version(1)
	.functionPublic("about", &Node::about)
	.functionPublic("reset", &Node::reset)
	.functionPublic("getPosition", &Node::getPosition)
	.functionPublic("setPosition", &Node::setPosition)
	.functionPublic("setParent", &Node::setParent)
	.functionPublic("getParent", &Node::getParent)
	.functionPublic("addChild", &Node::addChild)
	.functionPublic("getChild", &Node::getChild)
	.property("visibility", &Node::visibility)
	.property("Position", &Node::getPosition, &Node::setPosition)
	/*.editableProperty("position", &Node::position)*/
	.property("parent", &Node::parent);
END_DEFINE_META(Node)
namespace GFrameworkTest
{
	void IncrementGlobalCounter()
	{
		globalCounter++;
	}

	Node::Node(const char *_name, NodeSharedPtr& _parent) : GObject(_name)
	{	
		parent.setValue(_parent);
	}

	Node::~Node()
	{
	}

	void Node::setPosition(const glm::vec3& _pos)
	{
		position.setValue(_pos);
	}

	const glm::vec3 & Node::getPosition() const
	{
		return position.getValue();
	}

	void Node::about()
	{
		cout << "Type: " << "Node" << endl;
		cout << "Name: " << getName() << endl;
		cout << "Position: (" << position.getValue().x << "," << position.getValue().y << "," << position.getValue().z << ")" << endl;
		if(parent.getValue() != NULL)
			cout << "Parent: " << parent.getValue()->getName() << endl;
	}

	void Node::reset()
	{
		position = glm::vec3(0, 0, 0);
	}

	NodeSharedPtr Node::getChild(size_t index)
	{
		return children.at(index);
	}

	void Node::initialize()
	{

	}

	GMetaclass* Node::getMetaclass() const
	{
		return GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("node");
	}

	void Node::setParent(NodeSharedPtr _parent)
	{
		parent.setValue(_parent);
	}

	Node* Node::getParent()
	{
		return parent.getValue().get();
	}

	void Node::addChild(NodeSharedPtr _child)
	{
		children.push_back(_child);
	}

	const char* Node::getMagicString()
	{
		return "Mpfr";
	}
}

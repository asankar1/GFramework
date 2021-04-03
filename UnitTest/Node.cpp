#include <iostream>
#include "Node.h"
using namespace std;
using namespace GFramework;
using namespace GFrameworkTest;

//template class __declspec(dllexport) GMetaNonAbstractclass<GFrameworkTest::Node>;


	/*BEGIN_DEFINE_META(Node)
		GMetaclassList::instance().define<Node>("node")
			.baseMetaclass("GObject")
			.version(1)
			.function("about", &Node::about)
			.function("getPosition", &Node::getPosition)
			.function("setPosition", &Node::setPosition)
			.function("setParent", &Node::setParent)
			.function("getParent", &Node::getParent)
			.function("addChild", &Node::addChild)
			.function("getChild", &Node::getChild)
			.editableProperty("position", &Node::position)
			.property("parent", &Node::parent);
	END_DEFINE_META(Node)*/

	BEGIN_DEFINE_META(Node)
		GMetaNamespaceList::_global()._namespace("GFrameworkTest")
		.function("NodeFileInfo", NodeFileInfo)
		.define<Node>("node")
		.baseMetaclass("GObject", { "GFramework" })
		.version(1)
		.functionPublic("about", &Node::about)
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
	void NodeFileInfo()
	{
		std::cout << "NodeFileInfo\n";
	}
	Node::Node(const char *_name, NodeSharedPtr& _parent) : GObject(_name)
	{	
		parent.setValue(_parent.get());
		cout << "Node '" << getName() << "' constructed." << endl;
	}

	Node::~Node()
	{
		cout << "Node '" << getName() << "' destroyed." << endl;
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

	NodeSharedPtr Node::getChild(size_t index)
	{
		return children.at(index);
	}

	void Node::initialize()
	{
		cout << "Node '" << getName() << "' initialized." << endl;
	}

	GMetaclass* Node::getMetaclass()
	{
		return GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("node");
	}

	void Node::setParent(NodeSharedPtr _parent)
	{
		parent.setValue(_parent.get());
	}

	Node* Node::getParent()
	{
		return parent.getValue();
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

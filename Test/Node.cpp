#include <iostream>
#include "Node.h"
using namespace std;

namespace GFramework
{
	BEGIN_DEFINE_META(Node)
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
	END_DEFINE_META(Node)

	Node::Node(const char *_name, NodeSharedPtr& _parent) : GObject(_name)
	{	
		parent.setValue(_parent.get());
		cout << "Node '" << getName() << "' constructed." << endl;
	}

	Node::~Node()
	{
		cout << "Node '" << getName() << "' destroyed." << endl;
	}

	void Node::setPosition(float x, float y, float z)
	{
		cout << "Node '" << getName() << "' position set to " << x << "," << y << "," << z << endl;
		setPosition_(glm::vec3(x, y, z));
	}

	void Node::setPosition_(glm::vec3 && pos)
	{
		position.setValue(pos);
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

	const char * Node::metaclassName()
	{
		return GMetaclassList::instance().getMetaclassByType<Node>()->getName().c_str();
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

}
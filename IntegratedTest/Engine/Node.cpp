#include <memory>
#include <iostream>
#include "Node.h"
using namespace std;
using namespace GFramework;

template<>
GPropertyInterfaceSharedPtr GFramework::GPropertyUtility< std::shared_ptr<GFrameworkTest::Node> >::create()
{
    return std::make_shared< GPointerProperty<GFrameworkTest::Node> >();
}

template<>
GPropertyInterfaceSharedPtr GFramework::GPropertyUtility< shared_ptr<GFrameworkTest::Node> >::create(shared_ptr<GFrameworkTest::Node> value)
{
    return std::make_unique< GPointerProperty<GFrameworkTest::Node> >(value);
}

namespace GFrameworkTest
{
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
        .property("parent", &Node::getParent, &Node::setParent)
        //.property("position", &Node::getPosition, &Node::setPosition)
        .editableProperty("visibility", &Node::visibility)
        .editableProperty("position", &Node::position)
        ;
	END_DEFINE_META(Node)

	void NodeFileInfo()
	{
		std::cout << "NodeFileInfo\n";
	}
	GFrameworkTest::Node::Node(const char * _name) : GObject(_name)
	{
		cout << "Node '" << getName() << "' constructed." << endl;
	}
	Node::Node(const char *_name, NodeSharedPtr& _parent) : GObject(_name)
	{	
        parent.setValue(_parent);
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

    NodeSharedPtr Node::getChild(size_t index) const
	{
		return children.at(index);
	}

    size_t Node::getChildCount() const
    {
        return children.size();
    }

	void Node::initialize()
	{
		cout << "Node '" << getName() << "' initialized." << endl;
	}

    GMetaclass* Node::getMetaclass() const
	{
        //return GMetaNamespace::getMetaclassByType<Node>();
        return GET_METACLASS_INTERNAL(Node);
	}

	void Node::setParent(NodeSharedPtr _parent)
	{
        parent.setValue(_parent);
    }

    GFrameworkTest::NodeSharedPtr Node::getParent() const
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

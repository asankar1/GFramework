#include <functional>
#include <QAction>
#include <QApplication>
#include <QTreeView>
#include <testapplication.h>
#include <testproject.h>
#include <GEditor/utilities/logger.h>
#include <Engine/Node.h>

using namespace GFrameworkTest;
using namespace GFramework::Editor;
void addSphere();
void addNode();
class test
{
public:
    void callback()
    {
         Logger::debug() << "Cube addded";
    }
};

int main(int argc, char *argv[])
{
    qRegisterMetaType<GFramework::GObjectSharedPtr>("GObjectSharedPtr");
    qRegisterMetaTypeStreamOperators<GFramework::GObjectSharedPtr>("GObjectSharedPtr");
    TestApplication app(argc, argv);
	app.initialize();
    return app.exec();
}

void addNode()
{
	Logger::debug() << "Node addded";
	NodeSharedPtr np;
	GObjectSharedPtr object(new Node("Node1", np));
    //Application::instance()->getProject()->addObject(object, nullptr);
}


void addSphere()
{
    Logger::debug() << "Sphere addded";
    NodeSharedPtr np;
    GObjectSharedPtr object(new Node("Sphere1", np));
    //Application::instance()->getProject()->addObject(object, nullptr);
}

#include <GFramework/GReflection/GReflection.h>
#include <GEditor/utilities/logger.h>
#include <GEditor/ui/menus/contextmenu.h>
#include <GEditor/ui/windows/projectwindow.h>
#include <Engine/Node.h>

#include "testproject.h"
#include "testapplication.h"

using namespace GFramework;
using namespace GFramework::Editor;
using namespace GFrameworkTest;

TestApplication::TestApplication(int &argc, char **argv)
	: Application(argc, argv)
{
	//config.applicationName = "GFrameworkTestApp";
}

TestApplication::~TestApplication()
{

}

void TestApplication::initialize()
{
	appConfig->applicationName = "GFrameworkTestApp";

	Application::initialize();

    //create additional windows
    //nodeBrowserWindow = new ProjectWindow("Node Browser", mainWindow);
    //mainWindow->addDockWidget(Qt::LeftDockWidgetArea, nodeBrowserWindow);

	//register types
	auto m = GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("node");
    std::shared_ptr<GObjectEditorInfo> node_info = std::make_shared<GObjectEditorInfo>();
	node_info->contextMenu = std::make_shared<ContextMenu>();
    node_info->contextMenu->add("ResetNode", []() {
        for(auto var : Application::instance()->mainWindow->getDefaultProjectWindow()->getSelection())
		{
			Logger::debug() << "Node " << var->getName().c_str() << " reset";
		}
		
	});
    //Application::instance()->registerObjectType(m, QString(""));
    Application::instance()->registerObjectType(m, node_info);

	//create project window context menu
	auto prj_window = mainWindow->getDefaultProjectWindow();
	ContextMenu* mnu = prj_window->getContextMenu();
	mnu->add("Create/node", [] {	Logger::debug() << "Node addded";
    auto selection = Application::instance()->mainWindow->getDefaultProjectWindow()->getSelection();
	NodeSharedPtr np;
    if(selection.size() > 0)
    {
        //np = selection.back();
    }
	GObjectSharedPtr object(new Node("Node1", np));
    /*Application::instance()->getProject()->addObject(object);*/ });

	/*test t1;
	mnu->addActionHandler("Create/Cube", std::bind(&test::callback, &t1));
	mnu->add("Copy");
	mnu->add("Paste");*/
	
}

void TestApplication::newProject(QString path)
{
	auto file = new QFile(path);
	if (!file->open(QIODevice::ReadWrite)) {
		Logger::critical() << "Unable to open file " << path << " for writing!";
		return;
	}

	project = new TestProject(file);
	project->initialize();
}

void TestApplication::openProject(QString path)
{
	if (!QFile::exists(path))
	{
		Logger::critical() << "The project " << path << " does not exist!";
		return;
	}
	project = TestProject::openProject(path);
}

#include <memory>

#include <GFramework/GReflection/GReflection.h>
#include <GEditor/utilities/logger.h>
#include <GEditor/ui/menus/contextmenu.h>
#include <GEditor/ui/windows/projectwindow.h>


#include "testproject.h"
#include "testapplication.h"

using namespace std;
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
    nodeBrowserWindow = make_shared<TreeListWindow<Node>>("Node Browser", mainWindow);
    mainWindow->addWindow(nodeBrowserWindow.get(), Qt::LeftDockWidgetArea);

	//register types
	auto m = GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("node");
    std::shared_ptr<GObjectEditorInfo> node_info = std::make_shared<GObjectEditorInfo>();
	node_info->contextMenu = std::make_shared<ContextMenu>();
    node_info->contextMenu->add("ResetNode", []() {
        auto window = dynamic_cast<TreeListWindow<Node>*>(Application::instance()->mainWindow->getWindow("Node Browser"));
        for(auto var : window->getSelection())
		{
			Logger::debug() << "Node " << var->getName().c_str() << " reset";
		}
		
	});

    //Application::instance()->registerObjectType(m, QString(""));
    Application::instance()->registerObjectType(m, node_info);
#if 1
	//create project window context menu
    //auto prj_window = mainWindow->getDefaultProjectWindow();
    auto prj_window_ctx_mnu = nodeBrowserWindow->getContextMenu();

    prj_window_ctx_mnu->add("Create/Node", []
    {
        auto window = dynamic_cast<TreeListWindow<Node>*>(Application::instance()->mainWindow->getWindow("Node Browser"));
        if(window == nullptr)
        {
            Logger::critical() << "Unable to cast 'Node Browser' window when invoking content menu 'Create/Node'";
            return;
        }

        auto selection = window->getSelection();
        NodeSharedPtr parent;
        if(selection.size() > 0)
        {
            parent = dynamic_pointer_cast<Node>(selection.back());
        }
        else
        {
            auto project = dynamic_cast<TestProject*>(Application::instance()->getProject());
            parent = dynamic_pointer_cast<Node>(project->getSceneGraphRoot());
        }
        auto object = make_shared<Node>("DefaultNode", parent);
        auto section = dynamic_pointer_cast<ProjectSectionOfType<Node>>(Application::instance()->getProject()->getSection("SceneGraph"));
        if(!section)
        {
            Logger::critical() << "Unable to cast 'SceneGraph' section when invoking content menu 'Create/Node'";
            return;
        }
        section->addObject(object);
        Logger::debug() << "Node addded";
    });
#endif

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
    nodeBrowserWindow->setModel(project->getSection("SceneGraph")->getModel());
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

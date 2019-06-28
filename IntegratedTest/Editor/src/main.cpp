
#include <QAction>
#include <QApplication>
#include <QTreeView>
#include <GEditor/core/application.h>
#include <GEditor/core/project.h>
#include <GEditor/ui/menus/contextmenu.h>
#include <GEditor/ui/windows/projectwindow.h>
#include <Engine/Node.h>

using namespace GFrameworkTest;
using namespace GFramework::Editor;

int main(int argc, char *argv[])
{
    Application::Configuration config;
    config.applicationName = "GFrameworkTestApp";
    Application app(argc, argv, &config);
    app.initialize();

    QAction newAction("New");
    QAction createAction("Create");
    QAction copyAction("Copy");
    QAction pasteAction("paste");
    auto prj_window = app.mainWindow->getDefaultProjectWindow();
    ContextMenu* mnu = prj_window->getContextMenu();
    mnu->add("Create/Sphere");
    mnu->add("Create/Cube");
    mnu->add("Copy");
    mnu->add("Paste");
    return app.exec();
}

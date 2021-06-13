#include <QApplication>
#include <QTreeView>
#include <GFrameworkEditor/core/application.h>
#include <GFrameworkEditor/core/project.h>

using namespace GFramework::Editor;

int main(int argc, char *argv[])
{
    Application::Configuration config;
    config.applicationName = "GFrameworkTestApp";
    Application app(argc, argv, &config);
    app.initialize();

    /*MyModel myModel;
    auto view = new QTreeView();
    //treeWidget.setModel(model);
    view->setModel(&myModel);
    view->show();*/

    return app.exec();
}

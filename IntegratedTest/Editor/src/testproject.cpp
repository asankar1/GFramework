#include <memory>
#include "testproject.h"
#include <Engine/Node.h>
#include <GFramework/GReflection/GReflection.h>
#include <GEditor/utilities/logger.h>

using namespace std;
using namespace GFramework;
using namespace GFramework::Editor;
using namespace GFrameworkTest;

TestProject::TestProject(QFile* proj_file)
	: Project(proj_file)
{
    sceneGraphSection = nullptr;
	jsonDocument = QJsonDocument::fromJson(file->readAll());
    //rootJsonObject = jsonDocument.object();
    //GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("node")
    QStringList sceneGraphMimeTypes = {"application/gobject.node"};
    if(addSection("SceneGraph", sceneGraphMimeTypes, GMetaNamespaceList::_global()._namespace("GFrameworkTest").getMetaclass("node")))
    {
        sceneGraphSection = getSection("SceneGraph");
    }
    isModified = true;
}

TestProject::~TestProject()
{

}

void TestProject::initialize()
{
    sceneGraphRoot = make_shared<Node>("RootNode");
    sceneGraphSection->addObject(sceneGraphRoot);
    Project::initialize();
}

Project * TestProject::openProject(QString proj_path)
{
    auto input_file = new QFile(proj_path);
    if (!input_file->open(QIODevice::ReadWrite)) {
		Logger::critical() << "Unable to open file " << proj_path << " for writing!";
		return nullptr;
	}

    Project* prj = new TestProject(input_file);
    prj->loadSections();
	return prj;

}

GObjectSharedPtr TestProject::getSceneGraphRoot()
{
    return sceneGraphRoot;
}

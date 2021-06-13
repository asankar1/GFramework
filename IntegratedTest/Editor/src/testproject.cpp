#include <memory>
#include "testproject.h"
#include <Engine/Node.h>
#include <GEditor/utilities/logger.h>

using namespace std;
using namespace GFramework::Editor;
using namespace GFrameworkTest;

TestProject::TestProject(QFile* proj_file)
	: Project(proj_file)
{
    sceneGraphSection = nullptr;
	jsonDocument = QJsonDocument::fromJson(file->readAll());
    //rootJsonObject = jsonDocument.object();

    if(addSection<Node>("SceneGraph"))
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
}

Project * TestProject::openProject(QString proj_path)
{
	auto file = new QFile(proj_path);
	if (!file->open(QIODevice::ReadWrite)) {
		Logger::critical() << "Unable to open file " << proj_path << " for writing!";
		return nullptr;
	}

	Project* prj = new TestProject(file);
	return prj;

}

GObjectSharedPtr TestProject::getSceneGraphRoot()
{
    return sceneGraphRoot;
}

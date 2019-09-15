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
	jsonDocument = QJsonDocument::fromJson(file->readAll());
	rootJsonObject = jsonDocument.object();
	isModified = false;
}

TestProject::~TestProject()
{

}

void TestProject::initialize()
{
	root = make_shared<Node>("RootNode");
	addRootObject(root);
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

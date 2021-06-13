#pragma once
#include <testapplication.h>
#include <GEditor/core/project.h>

using namespace GFramework;
using namespace GFramework::Editor;


class TestProject : public Project
{
public:
	~TestProject();
	virtual void initialize() override;
	static Project* openProject(QString proj_path);
        GObjectSharedPtr getSceneGraphRoot();
private:
	TestProject(QFile* proj_file);
        std::shared_ptr<ProjectSection> sceneGraphSection;
        GObjectSharedPtr sceneGraphRoot;
        //TODO: see if the friend can be removed
	friend TestApplication;
};

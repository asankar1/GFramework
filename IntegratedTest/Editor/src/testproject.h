#pragma once
#include <testapplication.h>
#include <GEditor/core/project.h>

using namespace GFramework::Editor;


class TestProject : public Project
{
public:
	~TestProject();
	virtual void initialize() override;
	static Project* openProject(QString proj_path);

private:
	TestProject(QFile* proj_file);

	friend TestApplication;
};

#pragma once

#include <GEditor/core/application.h>
//#include <GEditor/ui/windows/projectwindow.h>

using namespace GFramework::Editor;

class TestApplication : public Application
{
public:
	TestApplication(int &argc, char **argv);
	~TestApplication();
	virtual void initialize() override;
	virtual void newProject(QString path) override;
	virtual void openProject(QString path) override;

private:
   //ProjectWindow* nodeBrowserWindow;
};

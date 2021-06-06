#pragma once

#include <Engine/Node.h>

#include <GEditor/core/application.h>
#include <GEditor/ui/windows/treelistwindow.h>

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
   std::shared_ptr<TreeListWindow<GFrameworkTest::Node>> nodeBrowserWindow;
};

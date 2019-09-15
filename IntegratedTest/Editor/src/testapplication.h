#pragma once

#include <GEditor/core/application.h>

using namespace GFramework::Editor;

class TestApplication : public Application
{
	Q_OBJECT

public:
	TestApplication(int &argc, char **argv);
	~TestApplication();
	virtual void initialize() override;
	virtual void newProject(QString path) override;
	virtual void openProject(QString path) override;

private:

};

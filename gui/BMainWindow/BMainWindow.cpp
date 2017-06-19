#include "BMainWindow.h"

#include "BRenderWidget\BRenderWidget.h"
#include "BSceneEditorWidget\BSceneEditorWidget.h"

BMainWindow::BMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// Menu bar SIGNAL-SLOT connections.
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(OnActionExit()));

	// Settings File.
	connect(ui.actionSave_Settings, SIGNAL(triggered()), ui.renderWidget, SLOT(OnSaveSettings()));
	connect(ui.actionLoad_Settings, SIGNAL(triggered()), ui.renderWidget, SLOT(OnLoadSettings()));
	connect(ui.actionSave_Settings_As, SIGNAL(triggered()), ui.renderWidget, SLOT(OnSaveSettingsAs()));

	// Render and Preview.
	connect(ui.actionRender, SIGNAL(triggered()),  ui.renderWidget, SLOT(OnRender()));
	connect(ui.actionPreview, SIGNAL(triggered()), ui.renderWidget, SLOT(OnPreview()));

	// Scene file (scene tab).
	connect(ui.actionNew_Scene, SIGNAL(triggered()),  ui.sceneEditor, SLOT(OnNewSceneFile()));
	connect(ui.actionOpen_Scene, SIGNAL(triggered()), ui.sceneEditor, SLOT(OnOpenSceneFile()));
	connect(ui.actionSave_Scene, SIGNAL(triggered()), ui.sceneEditor, SLOT(OnSaveSceneFile()));
	connect(ui.actionSave_Scene_As, SIGNAL(triggered()), ui.sceneEditor, SLOT(OnSaveSceneAs()));

	// Help > About..
	connect(ui.actionAbout, SIGNAL(triggered()),  this, SLOT(OnAbout()));
}

void BMainWindow::OnActionExit()
{
	// TODO. Clear data.
	exit(0);
}

void BMainWindow::OnAbout()
{

}
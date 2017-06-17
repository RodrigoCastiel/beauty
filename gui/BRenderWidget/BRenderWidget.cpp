#include "BRenderWidget.h"

BRenderWidget::BRenderWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	QString styleSheet = "background-color: #FFFFFF; color: #000000";
	ui.lineEdit_imgWidth->setStyleSheet(styleSheet);
	ui.lineEdit_imgWidth->setStyleSheet(styleSheet);
	ui.lineEdit_imgHeight->setStyleSheet(styleSheet);
	ui.lineEdit_recursionDepth->setStyleSheet(styleSheet);
	ui.lineEdit_sceneFile->setStyleSheet(styleSheet);
	ui.lineEdit_ConfigFile->setStyleSheet(styleSheet);
	ui.lineEdit_outputImg->setStyleSheet(styleSheet);

	// Setup SIGNAL-SLOT connections.

	// Settings File (push-buttons).
	connect(ui.pb_newConfigFile, SIGNAL(released()), this, SLOT(OnNewSettings()));
	connect(ui.pb_saveConfigFile, SIGNAL(released()), this, SLOT(OnSaveSettings()));
	connect(ui.pb_loadConfigFile, SIGNAL(released()), this, SLOT(OnLoadSettings()));

	// Render, Output Image and Scene file (push-buttons).
	connect(ui.pb_browseSceneFile, SIGNAL(released()), this, SLOT(OnSceneFileBrowse()));
	connect(ui.pb_browseOutputImg, SIGNAL(released()), this, SLOT(OnOutputImgBrowse()));
	connect(ui.pb_render, SIGNAL(released()), this, SLOT(OnRender()));
	connect(ui.pb_preview, SIGNAL(released()), this, SLOT(OnPreview()));
}

BRenderWidget::~BRenderWidget()
{
}

// === Settings File ====================================================================

void BRenderWidget::OnNewSettings()   // Push-button or action menu.
{

}

void BRenderWidget::OnSaveSettings()  // Push-button or action menu.
{

}

void BRenderWidget::OnLoadSettings()  // Push-button or action menu.
{

}

// === Render, Output Image and Scene file. =============================================

void BRenderWidget::OnSceneFileBrowse()
{

}

void BRenderWidget::OnOutputImgBrowse()
{

}

void BRenderWidget::OnRender()
{

}

void BRenderWidget::OnPreview()
{

}
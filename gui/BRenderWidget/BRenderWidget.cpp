#include "BRenderWidget.h"

#include <QFileDialog>
#include <QMessageBox>

BRenderWidget::BRenderWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	mSettingsFileLoaded = false;
	
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

void BRenderWidget::OnSaveSettings()
{
	if (!mSettingsFileLoaded)
	{
		QString filePath = QFileDialog::getSaveFileName(this, tr("Save .config file"), 
														"", tr("CONFIG (*.config);"));

		if (filePath.isEmpty())
			return;

		ui.lineEdit_ConfigFile->setText(filePath);
		mSettingsFileLoaded = true;
	}

	QString filePath = ui.lineEdit_ConfigFile->text();

	// TODO: write data into config. file.
}

void BRenderWidget::OnLoadSettings()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open .config file"), 
													"", tr("CONFIG (*.config);"));

	if (filePath.isEmpty())
		return;

	ui.lineEdit_ConfigFile->setText(filePath);
	mSettingsFileLoaded = true;

	// TODO: load data into GUI (line edits).
}

// === Render, Output Image and Scene file. =============================================

void BRenderWidget::OnSceneFileBrowse()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open .scene file"), "", tr("SCENE (*.scene);"));

	if (!filePath.isEmpty())
		ui.lineEdit_sceneFile->setText(filePath);
}

void BRenderWidget::OnOutputImgBrowse()
{
	QString filePath = QFileDialog::getSaveFileName(this, tr("Render image file"), "", tr("JPEG (*.jpg); PNG (*.png)"));
	
	if (!filePath.isEmpty())
		ui.lineEdit_outputImg->setText(filePath);
}

void BRenderWidget::OnRender()
{

}

void BRenderWidget::OnPreview()
{

}
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
	// TODO.
}

BRenderWidget::~BRenderWidget()
{
}

#pragma once

#include <QWidget>
#include "ui_BRenderWidget.h"

class BRenderWidget : public QWidget
{
	Q_OBJECT

public:
	BRenderWidget(QWidget *parent = Q_NULLPTR);
	~BRenderWidget();

private:
	Ui::BRenderWidget ui;

public slots:
		

	// Settings file. 
	void OnNewSettings();   // Push-button or action menu.
	void OnSaveSettings();  // Push-button or action menu.
	void OnLoadSettings();  // Push-button or action menu.

	// Render, Output Image and Scene file.
	void OnSceneFileBrowse();
	void OnOutputImgBrowse();
	void OnRender();
	void OnPreview();
 
};

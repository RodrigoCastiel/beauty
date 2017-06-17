#pragma once

#include <QWidget>
#include "ui_BSceneEditorWidget.h"

class BSceneEditorWidget : public QWidget
{
	Q_OBJECT

public:
	BSceneEditorWidget(QWidget *parent = Q_NULLPTR);
	~BSceneEditorWidget();

private:
	Ui::BSceneEditorWidget ui;

public slots:

	// Scene file methods.
	void OnNewSceneFile();
	void OnOpenSceneFile();
	void OnSaveSceneFile();
};

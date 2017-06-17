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
};

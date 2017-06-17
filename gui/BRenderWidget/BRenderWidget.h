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
};

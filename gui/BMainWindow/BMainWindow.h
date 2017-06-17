#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BMainWindow.h"

class BMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	BMainWindow(QWidget *parent = Q_NULLPTR);

private:
	Ui::BMainWindowClass ui;

public slots:
	// Menu bar slots.

	// Quit.
	void OnActionExit();  

};

#include "BMainWindow.h"

BMainWindow::BMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// Menu bar SIGNAL-SLOT connections.
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(OnActionExit()));

}

void BMainWindow::OnActionExit()
{
	// TODO. Clear data.
	exit(0);
}
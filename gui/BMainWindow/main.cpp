#include "BMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BMainWindow w;
	w.show();
	return a.exec();
}

#include "QtDirect3D12.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtDirect3D12 w;
	w.show();
	return a.exec();
}

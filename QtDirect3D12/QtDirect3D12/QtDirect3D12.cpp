#include <qevent.h>
#include "QtDirect3D12.h"

QtDirect3D12::QtDirect3D12(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void QtDirect3D12::resizeEvent(QResizeEvent *event)
{
	QSize size = event->size();
}

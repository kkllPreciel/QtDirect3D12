#include <qevent.h>
#include "QtDirect3D12.h"
#include "../Sein/Direct3D12Device.h"

QtDirect3D12::QtDirect3D12(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	auto pDevice = new Sein::Direct3D12::Device();

	// TODO:reinterpret_cast‚ðíœ‚·‚é
	pDevice->Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());

	delete pDevice;
}

void QtDirect3D12::resizeEvent(QResizeEvent *event)
{
	QSize size = event->size();
}

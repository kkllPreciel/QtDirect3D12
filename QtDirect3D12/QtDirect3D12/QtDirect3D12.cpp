#include <qevent.h>
#include <qtimer.h>
#include "QtDirect3D12.h"
#include "../Sein/Direct3D12Device.h"

QtDirect3D12::QtDirect3D12(QWidget *parent)
	: QWidget(parent),
	timer(new QTimer)
{
	ui.setupUi(this);

	// DirectX12のラッパーを作成
	// TODO:reinterpret_castを削除する
	auto pDevice = new Sein::Direct3D12::Device();
	pDevice->Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());
	delete pDevice;

	// メインループ呼び出し設定
	connect(timer, SIGNAL(timeout()), this, SLOT(mainLoop()));
	timer->start(1000 / 60);
}

QtDirect3D12::~QtDirect3D12()
{
	if (timer->isActive())
	{
		timer->stop();
	}

	delete timer;
}

void QtDirect3D12::mainLoop()
{
	OutputDebugString(L"exe\n");
}

void QtDirect3D12::resizeEvent(QResizeEvent *event)
{
	QSize size = event->size();
}

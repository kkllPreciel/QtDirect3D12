#include <qevent.h>
#include <qtimer.h>
#include <Sein\Direct3D12\Direct3D12Device.h>
#include <Sein\Direct3D12\VertexBuffer.h>
#include <Sein\Direct3D12\IndexBuffer.h>
#include <Sein\Direct3D12\InstanceBuffer.h>
#include <Sein\PMX\Loader.h>
#include "QtDirect3D12.h"

QtDirect3D12::QtDirect3D12(QWidget *parent)
	: QWidget(parent),
	timer(new QTimer),
	device(new Sein::Direct3D12::Device()),
	vertexBuffer(new Sein::Direct3D12::VertexBuffer),
	indexBuffer(new Sein::Direct3D12::IndexBuffer),
	instanceBuffer(new Sein::Direct3D12::InstanceBuffer)
{
	ui.setupUi(this);

	// DirectX12のラッパーを作成
	// TODO:reinterpret_castを削除する
	device->Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());
	Sein::Pmx::Loader loader;
	loader.Load("../Resources/Pmx/YYB式桜ミクv1.00.pmx");
	//loader.Load("D:/DiskD/Study/Qt/QtDirect3D12/QtDirect3D12/Resources/Pmx/Tda式初音ミク・アペンド_Ver1.10.pmx");
	vertexBuffer->Create(&(device->GetDevice()), loader.GetVertexSize() * loader.GetVertexCount(), loader.GetVertexSize(), loader.GetVertices());
	indexBuffer->Create(&(device->GetDevice()), loader.GetIndexSize() * loader.GetIndexCount(), loader.GetIndices(), DXGI_FORMAT_R32_UINT);

	// メインループ呼び出し設定
	connect(timer.get(), SIGNAL(timeout()), this, SLOT(mainLoop()));
	timer->start(1000 / 60);
}

QtDirect3D12::~QtDirect3D12()
{
	if (timer->isActive())
	{
		timer->stop();
	}
}

void QtDirect3D12::mainLoop()
{
	device->BeginScene();
	device->Render(*vertexBuffer, *indexBuffer, *instanceBuffer);
	device->EndScene();
	device->Present();
}

void QtDirect3D12::resizeEvent(QResizeEvent *event)
{
	QSize size = event->size();
}

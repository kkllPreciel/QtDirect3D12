#include <qevent.h>
#include <qtimer.h>
#include "QtDirect3D12.h"
#include "Sein/Direct3D12/Direct3D12Device.h"
#include "Sein/PMX/Loader.h"

QtDirect3D12::QtDirect3D12(QWidget *parent)
	: QWidget(parent),
	timer(new QTimer),
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{
	ui.setupUi(this);

	// DirectX12のラッパーを作成
	// TODO:reinterpret_castを削除する
	device = new Sein::Direct3D12::Device();
	device->Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());
	Sein::Pmx::Loader loader;
	loader.Load("D:/DiskD/Study/Qt/QtDirect3D12/QtDirect3D12/Resources/Pmx/YYB式桜ミクv1.00.pmx");
	vertexBuffer = new Sein::Direct3D12::VertexBuffer;
	indexBuffer = new Sein::Direct3D12::IndexBuffer;
	vertexBuffer->Create(&(device->GetDevice()), loader.GetVertexSize() * loader.GetVertexCount(), loader.GetVertexSize(), loader.GetVertices());
	indexBuffer->Create(&(device->GetDevice()), loader.GetIndexSize() * loader.GetIndexCount(), loader.GetIndices(), DXGI_FORMAT_R8_UINT);

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
	delete device;
}

void QtDirect3D12::mainLoop()
{
	device->BeginScene();
	device->Render(*vertexBuffer, *indexBuffer);
	device->EndScene();
	device->Present();
}

void QtDirect3D12::resizeEvent(QResizeEvent *event)
{
	QSize size = event->size();
}

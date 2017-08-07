#include <qevent.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <Sein\Direct3D12\direct3d12_device.h>
#include <Sein\Direct3D12\vertex_buffer.h>
#include <Sein\Direct3D12\index_buffer.h>
#include <Sein\PMX\loader.h>
#include "QtDirect3D12.h"

QtDirect3D12::QtDirect3D12(QWidget *parent)
  : QWidget(parent),
  timer(new QTimer),
  device(new Sein::Direct3D12::Device()),
  vertexBuffer(new Sein::Direct3D12::VertexBuffer),
  indexBuffer(new Sein::Direct3D12::IndexBuffer)
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
  static QTime time;
  static auto frameCount = 0;
  wchar_t buf[256] = { 0 };

  swprintf_s<256>(buf, L"FPS is %f\n", (float)(frameCount / (time.elapsed() / 1000.0f)));

  if (frameCount == 0)
    time.start();
  else
  {
    OutputDebugString(buf);
  }

  frameCount++;

  device->BeginScene();
  device->Render(*vertexBuffer, *indexBuffer);
  device->EndScene();
  device->Present();
}

void QtDirect3D12::resizeEvent(QResizeEvent *event)
{
  QSize size = event->size();
}
#include <qevent.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <Sein\Direct3D12\direct3d12_device.h>
#include <Sein\Direct3D12\vertex_buffer.h>
#include <Sein\Direct3D12\index_buffer.h>
#include <Sein\Direct3D12\constant_buffer.h>
#include <Sein\Direct3D12\shader_resource_buffer.h>
#include <Sein\PMX\loader.h>
#include "QtDirect3D12.h"
#include <qimage.h>
#include <qmimedata.h>
#include <qprogressdialog.h>

QtDirect3D12::QtDirect3D12(QWidget *parent)
  : QWidget(parent),
  timer(std::make_unique<QTimer>()),
  device(std::make_unique<Sein::Direct3D12::Device>()),
  vertexBuffer(std::make_unique<Sein::Direct3D12::VertexBuffer>()),
  indexBuffer(std::make_unique<Sein::Direct3D12::IndexBuffer>()),
  constantBufferView(nullptr),
  shaderResourceBuffer(nullptr)
{
  ui.setupUi(this);

  // �h���b�v��������
  setAcceptDrops(true);

  // DirectX12�̃��b�p�[���쐬
  // TODO:reinterpret_cast���폜����
  device->Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());
  Sein::Pmx::Loader loader;
  loader.Load("../Resources/Pmx/YYB�����~�Nv1.00.pmx");
  vertexBuffer->Create(&(device->GetDevice()), loader.GetVertexSize() * loader.GetVertexCount(), loader.GetVertexSize(), loader.GetVertices());
  indexBuffer->Create(&(device->GetDevice()), loader.GetIndexSize() * loader.GetIndexCount(), loader.GetIndices(), DXGI_FORMAT_R32_UINT);

  // �萔�o�b�t�@���쐬
  constantBufferView = device->CreateConstantBuffer(sizeof(ConstantBufferType));

  {
    // �V�F�[�_�[���\�[�X�o�b�t�@���쐬
    shaderResourceBuffer = device->CreateShaderResourceBuffer(INSTANCE_NUM, sizeof(InstanceBuffer));

    // �C���X�^���X�ʂ̃f�[�^��������
    instanceBufferData.resize(INSTANCE_NUM);
    DirectX::XMStoreFloat4x4(&(instanceBufferData[0].world), DirectX::XMMatrixIdentity());
    DirectX::XMStoreFloat4x4(&(instanceBufferData[1].world), DirectX::XMMatrixTranslation(1.0f, 1.0f, 1.0f));
    DirectX::XMStoreFloat4x4(&(instanceBufferData[2].world), DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationZ(DirectX::XM_PI), DirectX::XMMatrixTranslation(2.0f, 2.0f, 2.0f)));
    DirectX::XMStoreFloat4x4(&(instanceBufferData[3].world), DirectX::XMMatrixTranslation(-1.0f, -1.0f, -1.0f));
    DirectX::XMStoreFloat4x4(&(instanceBufferData[4].world), DirectX::XMMatrixTranslation(-2.0f, -2.0f, -2.0f));
    shaderResourceBuffer->Map(&instanceBufferData[0], sizeof(InstanceBuffer) * instanceBufferData.size());
  }

  QImage image;
  if (image.load("../Resources/Texture/A.png"))
  {
    auto format = image.format();
    auto bytesPerLine = image.bytesPerLine();
    auto width = image.width();
    auto height = image.height();
    auto buffer = std::make_unique<uchar[]>(bytesPerLine * height);
    std::memcpy(buffer.get(), image.convertToFormat(QImage::Format::Format_RGBA8888).constBits(), bytesPerLine * height);

    // �e�N�X�`���o�b�t�@���쐬
    device->CreateTextureBuffer(buffer.get(), width, height, 4);

    buffer.reset(nullptr);
  }

  // ���C�����[�v�Ăяo���ݒ�
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
  // �f�o�b�O�pFPS�\��
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
  }

  // �萔�o�b�t�@���X�V
  {
    static float now = 0.0f;
    static float angle = DirectX::XM_PI / 180.0f;

    // ��]
    now += angle;

    // ���[���h�s����X�V
    DirectX::XMStoreFloat4x4(&(constantBuffer.world), DirectX::XMMatrixRotationY(now));

    // �r���[�s����쐬
    DirectX::XMVECTORF32 eye = { 0.0f, 10.0f, -30.5f, 0.0f };
    DirectX::XMVECTORF32 at = { 0.0f, 10.0f, 0.0f, 0.0f };
    DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
    DirectX::XMStoreFloat4x4(&(constantBuffer.view), DirectX::XMMatrixLookAtLH(eye, at, up));

    // �v���W�F�N�V�����s����쐬
    DirectX::XMStoreFloat4x4(&(constantBuffer.projection), DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 3.0f, 600.0f / 400.0f, 0.1f, 1000.0f));

    // �萔�o�b�t�@���X�V
    constantBufferView->Map(&constantBuffer, sizeof(ConstantBufferType));
  }

  device->BeginScene();
  //device->Render(*vertexBuffer, *indexBuffer, instanceBufferData.size());
  device->Render(*vertexBuffer, *indexBuffer, 1);
  device->EndScene();
  device->Present();
}

void QtDirect3D12::resizeEvent(QResizeEvent *event)
{
  QSize size = event->size();
}

void QtDirect3D12::dragEnterEvent(QDragEnterEvent *event)
{
    // �t�@�C���`���̂ݎ󂯕t����
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

#include "../Loader/obj_loader.h"

void QtDirect3D12::dropEvent(QDropEvent* event)
{
    QString file = event->mimeData()->urls().first().toLocalFile();

    auto loader = App::Loader::Obj::CreateLoader();
    loader->Load(file.toLocal8Bit().toStdString(), nullptr);

    QProgressDialog progress(
        u8"���΂炭���҂����������E�E�E",
        u8"�L�����Z��",
        0,
        0,
        this,
        0
    );
    progress.setFixedSize(progress.sizeHint());     // �E�B���h�E�����T�C�Y�ł��Ȃ��悤�ɂ���
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle(u8"������");
    progress.show();

    for (int cnt = 0; cnt<200; cnt += 20) {
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        progress.setValue(cnt);
        if (progress.wasCanceled()) {
            break;
        }
        Sleep(500);
    }
    progress.setValue(200);
}
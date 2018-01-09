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
#include "./Loader/obj_loader.h"

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

  // ���_�E�����_��������
  eye_ = { 0.0f, 10.f, -30.5f };
  at_ = { 0.0f, 10.0f, 0.0f };

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
    DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&eye_);
    DirectX::XMVECTOR at = DirectX::XMLoadFloat3(&at_);
    DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
    DirectX::XMStoreFloat4x4(&(constantBuffer.view), DirectX::XMMatrixLookAtLH(eye, at, up));

    // �v���W�F�N�V�����s����쐬
    DirectX::XMStoreFloat4x4(&(constantBuffer.projection), DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 3.0f, 600.0f / 400.0f, 0.1f, 1000.0f));

    // �萔�o�b�t�@���X�V
    constantBufferView->Map(&constantBuffer, sizeof(ConstantBufferType));
  }

  auto index_count = 321567;
  if (model_ != nullptr)
  {
    index_count = model_->GetIndexCount();
  }

  device->BeginScene();
  //device->Render(*vertexBuffer, *indexBuffer, instanceBufferData.size());
  device->Render(*vertexBuffer, *indexBuffer, index_count, 1);
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

void QtDirect3D12::dropEvent(QDropEvent* event)
{
  // �ǂݍ��ݒ��_�C�A���O��\��
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
  progress.setValue(100);

  QString file = event->mimeData()->urls().first().toLocalFile();
  auto loader = App::Loader::Obj::CreateLoader();
  const auto model = loader->Load(file.toLocal8Bit().toStdString(), nullptr);

  // �A���C�������g��1�o�C�g�ɐݒ�
#pragma pack(push, 1)
  // �A���C�������g��1�o�C�g�ɐݒ�
  struct Vertex
  {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 texcoord;
  };
#pragma pack(pop)


  auto vertex_size = sizeof(Vertex);
  std::vector<Vertex> vertices(model->GetVertexCount());
  const auto base_vertices = model->GetVertices();

  auto begin_iterator = vertices.begin();
  // TODO:range-based for�ɂ���(ID���o��boost::adaptors:indexed���g�p����)
  for (auto iterator = vertices.begin(); iterator != vertices.end(); ++iterator)
  {
    auto index = std::distance(begin_iterator, iterator);
    iterator->position.x = base_vertices[index].x;
    iterator->position.y = base_vertices[index].y;
    iterator->position.z = base_vertices[index].z;
    iterator->normal.x = iterator->normal.y = iterator->normal.z = 1.0f;
    iterator->texcoord.x = iterator->texcoord.y = 0.5f;
  }

  const auto indices = model->GetIndices();

  // ���_�f�[�^�A�C���f�b�N�X�o�b�t�@�̍X�V
  vertexBuffer->Create(&(device->GetDevice()), vertex_size * model->GetVertexCount(), vertex_size, &(vertices[0]));
  indexBuffer->Create(&(device->GetDevice()), sizeof(uint32_t) * model->GetIndexCount(), &(indices[0]), DXGI_FORMAT_R32_UINT);
}

void QtDirect3D12::wheelEvent(QWheelEvent* event)
{
  QPoint degrees = event->angleDelta() / 8;

  // ���_���璍���_�ւ̃x�N�g�����쐬
  DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&eye_);
  DirectX::XMVECTOR at = DirectX::XMLoadFloat3(&at_);
  DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(at, eye);
  
  // ���_���璍���_�ւ̋������擾
  float distance = 0.0f;
  DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(dir));

  // TODO:��苗���ȏ�͋߂Â��Ȃ��悤�ɂ���
  // TODO:�����_�Ƃ̋����ňړ��ʂɕ␳������
  DirectX::XMStoreFloat3(&eye_, DirectX::XMVectorAdd(eye, DirectX::XMVectorScale(DirectX::XMVector3Normalize(dir), degrees.y() / std::abs(degrees.y()) * distance)));
}
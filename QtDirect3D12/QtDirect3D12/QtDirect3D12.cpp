// TODO:include�̏������R�[�f�B���O���[���ɍ��킹��
#include <qevent.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <Sein\Direct3D12\vertex_buffer.h>
#include <Sein\Direct3D12\index_buffer.h>
#include <Sein\Direct3D12\constant_buffer.h>
#include <Sein\Direct3D12\shader_resource_buffer.h>
#include <Sein\PMX\loader.h>
#include "QtDirect3D12.h"
#include <qimage.h>
#include <qmimedata.h>
#include <qprogressdialog.h>
#include "actor/camera_move_component.h"
#include "actor/camera_component.h"
#include "job_system/job_scheduler.h"
#include "job_system/async_job_manager.h"

QtDirect3D12::QtDirect3D12(QWidget *parent)
  : QWidget(parent),
  timer(std::make_unique<QTimer>()),
  vertexBuffer(std::make_unique<Sein::Direct3D12::VertexBuffer>()),
  indexBuffer(std::make_unique<Sein::Direct3D12::IndexBuffer>()),
  constantBufferView(nullptr),
  shaderResourceBuffer(nullptr)
{
  ui.setupUi(this);

  // �h���b�v��������
  setAcceptDrops(true);

  // �f�o�C�X�̍쐬
  // DirectX12�̃��b�p�[���쐬
  // TODO:reinterpret_cast���폜����
  auto device = std::make_unique<Sein::Direct3D12::Device>();
  device->Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());  

  Sein::Pmx::Loader loader;
  loader.Load("../Resources/Pmx/YYB�����~�Nv1.00.pmx");
  vertexBuffer->Create(&(device->GetDevice()), loader.GetVertexSize() * loader.GetVertexCount(), loader.GetVertexSize(), loader.GetVertices());
  indexBuffer->Create(&(device->GetDevice()), loader.GetIndexSize() * loader.GetIndexCount(), loader.GetIndices(), DXGI_FORMAT_R32_UINT);
  index_count_ = 321567;

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

  // �x�[�X�J���[(�A���x�h)
  if (image.load("../Resources/Texture/marble-speckled-albedo.png"))
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

  // ���^���b�N(������)
  if (image.load("../Resources/Texture/marble-speckled-metalness.png"))
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

  // ���t�l�X(�e��)
  if (image.load("../Resources/Texture/marble-speckled-roughness.png"))
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

  // �v���~�e�B�u�^�C�v�̐ݒ�
  topology_ = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

  // �񓯊��W���u�̏���
  App::job_system::AsyncJobManager::GetInstance()->Create();

  // ���x���̍쐬
  level_ = std::make_unique<App::actor::ViewerLevel>();
  level_->SetDevice(std::move(device));
  level_->Create();

  is_loading = false;

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
  if (is_loading)
  {
    return;
  }

  // �f�o�b�O�pFPS�\��
  {
#if false
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
#endif
  }

  // �萔�o�b�t�@���X�V
  {
    // ���[���h�s����X�V
    DirectX::XMStoreFloat4x4(&(constantBuffer.world), DirectX::XMMatrixRotationQuaternion(level_->GetModelQuaternion()));

    // �W���u���s
    App::job_system::JobScheduler::GetInstance()->Execute(6);

    // �r���[�s����쐬
    DirectX::XMVECTOR eye = level_->GetCameraPosition();
    DirectX::XMVECTOR at = level_->GetLookAt();
    DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
    DirectX::XMStoreFloat4x4(&(constantBuffer.view), DirectX::XMMatrixLookAtLH(eye, at, up));

    // �v���W�F�N�V�����s����쐬
    DirectX::XMStoreFloat4x4(&(constantBuffer.projection), DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 3.0f, 600.0f / 400.0f, 0.1f, 1000.0f));

    // �萔�o�b�t�@���X�V
    constantBufferView->Map(&constantBuffer, sizeof(ConstantBufferType));
  }

  decltype(auto) device = level_->GetDevice();
  device.BeginScene();
  device.SetPrimitiveTopology(topology_);
  device.SetVertexBuffers(0, 1, &(vertexBuffer->GetView()));
  device.SetIndexBuffer(&(indexBuffer->GetView()));
  device.Render(index_count_, 1);
  device.EndScene();
  device.Present();
}

void QtDirect3D12::resizeEvent(QResizeEvent *event)
{
  // TODO:�E�B���h�E�̃��T�C�Y�ɑΉ�����
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

  level_->RegisterLoadedEvent([&](App::IAppModel* model) {

    decltype(auto) device = level_->GetDevice();
    decltype(auto) vertices = model->GetVertices();
    decltype(auto) indices = model->GetIndices();

    // ���_�f�[�^�A�C���f�b�N�X�o�b�t�@�̍X�V
    auto vertex_size = sizeof(App::IAppModel::Vertex);
    vertexBuffer->Create(&(device.GetDevice()), vertex_size * vertices.size(), vertex_size, &(vertices[0]));
    indexBuffer->Create(&(device.GetDevice()), sizeof(uint32_t) * indices.size(), &(indices[0]), DXGI_FORMAT_R32_UINT);
    index_count_ = indices.size();

    is_loading = false;
  });

  is_loading = true;

  // �񓯊��ǂݍ���
  QString file = event->mimeData()->urls().first().toLocalFile();
  level_->LoadModel(file.toLocal8Bit().toStdString());
}

void QtDirect3D12::wheelEvent(QWheelEvent* event)
{
  // �����_�Ƃ̋���

  QPoint degrees = event->angleDelta() / 8;
  const auto force = degrees.y() / std::abs(degrees.y());

  // ���x���Ƀz�C�[����]�C�x���g��ʒm����
  level_->DispatchWheelEvent(force);

  // TODO:���炩�Ɉړ�����悤�ɂ���(�W���u�ɓo�^���Ĉړ�������?)
  // ���݂̎��_�ƈړ���̎��_����`��ԂŎw�莞�Ԃňړ����s���悤�ɂ���
  // �ړ����Ɉړ���̎��_���X�V�����\�����l������
  // ���C�����[�v�̃W���u���s������
  // 1.�t�@�C���ǂݍ���(�񓯊��W���u)
  // 2.���W�X�V
  // 3.�`��ɂ���
}

void QtDirect3D12::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key::Key_Shift)
  {
    if (topology_ == D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST)
    {
      topology_ = D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }
    else
    {
      topology_ = D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
    }
  }
}
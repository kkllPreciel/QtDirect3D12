// TODO:include�̏������R�[�f�B���O���[���ɍ��킹��
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
#include <unordered_map>
#include <boost/range/adaptor/indexed.hpp>
#include "actor/camera_move_component.h"
#include "actor/camera_component.h"
#include "job_system/job_scheduler.h"

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

  // ���_�E�����_��������
  camera_ = std::make_unique<App::actor::Actor>();
  camera_->AddComponent<App::actor::CameraComponent>()->SetLookAt({ 0.0f, 10.0f, 0.0f });
  camera_->SetPosition({ 0.0f, 10.0f, -30.5f });

  // �v���~�e�B�u�^�C�v�̐ݒ�
  topology_ = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

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
    static float now = 0.0f;
    static float angle = DirectX::XM_PI / 180.0f;

    // ��]
    now += angle;

    // ���[���h�s����X�V
    DirectX::XMStoreFloat4x4(&(constantBuffer.world), DirectX::XMMatrixRotationY(now));

    // �W���u���s
    App::job_system::JobScheduler::GetInstance()->Execute(6);

    // �r���[�s����쐬
    DirectX::XMVECTOR eye = camera_->GetPosition();
    DirectX::XMVECTOR at = camera_->GetComponent<App::actor::CameraComponent>()->GetLookAt();
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
    // TODO:�������C���f�b�N�X���ɏC������
    index_count = model_->GetIndexCount();
  }

  device->BeginScene();
  device->SetPrimitiveTopology(topology_);
  device->SetVertexBuffers(0, 1, &(vertexBuffer->GetView()));
  device->SetIndexBuffer(&(indexBuffer->GetView()));
  device->Render(index_count, 1);
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
  model_ = loader->Load(file.toLocal8Bit().toStdString(), nullptr);

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

  std::vector<Vertex> vertices;
  std::unordered_map<std::string, uint32_t> map;
  std::vector<uint32_t> indices;

  const auto points = model_->GetControlPoints();
  const auto normals = model_->GetNormals();
  const auto tex_coords = model_->GetTextureCoords();
  const auto point_indices = model_->GetIndices();
  const auto normal_indices = model_->GetNormalIndices();
  const auto tex_coords_indices = model_->GetTextureCoordIndices();

  // ���_�f�[�^���X�g�ƁA���_�f�[�^�C���f�b�N�X�ƃn�b�V���������X�g���쐬����
  for (const auto point : point_indices | boost::adaptors::indexed())
  {
    const auto index = point.index();

    // �n�b�V�����쐬
    std::string hash = std::to_string(point.value());

    if (tex_coords_indices.empty() == false)
    {
      hash += '-' + std::to_string(tex_coords_indices[index]);
    }

    if (normal_indices.empty() == false)
    {
      hash += '-' + std::to_string(normal_indices[index]);
    }

    // �n�b�V�������݂��Ȃ��ꍇ�͒��_�f�[�^��ǉ�����
    if (map.count(hash) == 0)
    {
      // ���_�f�[�^
      Vertex vertex = {};
      vertex.position = points.at(point.value());

      if (normal_indices.empty() == false)
      {
        vertex.normal = normals.at(normal_indices.at(index));
      }

      if (tex_coords_indices.empty() == false)
      {
        vertex.texcoord = tex_coords.at(tex_coords_indices.at(index));
      }

      vertices.emplace_back(vertex);

      // �n�b�V�����X�g���X�V
      map.insert({ hash, static_cast<uint32_t>(vertices.size() - 1) });
    }

    // �C���f�b�N�X���X�g�ɃC���f�b�N�X��ǉ�����
    indices.emplace_back(map.at(hash));
  }

  // ���_�f�[�^�A�C���f�b�N�X�o�b�t�@�̍X�V
  auto vertex_size = sizeof(Vertex);
  vertexBuffer->Create(&(device->GetDevice()), vertex_size * vertices.size(), vertex_size, &(vertices[0]));
  indexBuffer->Create(&(device->GetDevice()), sizeof(uint32_t) * indices.size(), &(indices[0]), DXGI_FORMAT_R32_UINT);
}

void QtDirect3D12::wheelEvent(QWheelEvent* event)
{
  constexpr auto value = 1.0f;

  QPoint degrees = event->angleDelta() / 8;

  // ���_���璍���_�ւ̃x�N�g�����쐬
  DirectX::XMVECTOR eye = camera_->GetPosition();
  DirectX::XMVECTOR at = camera_->GetComponent<App::actor::CameraComponent>()->GetLookAt();
  DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(at, eye);
  
  // ���_���璍���_�ւ̋������擾
  float distance = 0.0f;
  DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(dir));

  // ����������Ă���ꍇ�͈ړ��ʂɕ␳���s��
  const auto move = value * (std::fabsf(distance) * 0.5);

  const auto force = degrees.y() / std::abs(degrees.y());
  DirectX::XMVECTOR target = DirectX::XMVectorAdd(eye, DirectX::XMVectorScale(DirectX::XMVector3Normalize(dir), force * move));

  // �����_�Ƃ̋��������ȉ��Ȃ�߂Â��Ȃ��悤�ɂ���
  dir = DirectX::XMVectorSubtract(at, target);
  distance = 0.0f;
  DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(dir));
  if (distance < 1.0f)
  {
    return;
  }

  decltype(auto) component = camera_->GetComponent<App::actor::CameraMoveComponent>();
  if (component == nullptr)
  {
    component = camera_->AddComponent<App::actor::CameraMoveComponent>();
  }
  
  component->SetBeginPosition(eye);
  component->SetEndPosition(target);
  component->SetTime(1);
  component->Create();

  // TODO:���炩�Ɉړ�����悤�ɂ���(�W���u�ɓo�^���Ĉړ�������?)
  // ���݂̎��_�ƈړ���̎��_����`��ԂŎw�莞�Ԃňړ����s���悤�ɂ���
  // �ړ����Ɉړ���̎��_���X�V�����\�����l������
  // ���C�����[�v�̃W���u���s������
  // 1.�t�@�C���ǂݍ���(�񓯊��W���u)
  // 2.���W�X�V
  // 3.�`��ɂ���
  // DirectX::XMStoreFloat3(&eye_, eye);
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
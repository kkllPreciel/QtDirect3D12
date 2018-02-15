// TODO:includeの順序をコーディングルールに合わせる
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

  // ドロップを許可する
  setAcceptDrops(true);

  // デバイスの作成
  // DirectX12のラッパーを作成
  // TODO:reinterpret_castを削除する
  auto device = std::make_unique<Sein::Direct3D12::Device>();
  device->Create(reinterpret_cast<HWND>(this->winId()), this->width(), this->height());  

  Sein::Pmx::Loader loader;
  loader.Load("../Resources/Pmx/YYB式桜ミクv1.00.pmx");
  vertexBuffer->Create(&(device->GetDevice()), loader.GetVertexSize() * loader.GetVertexCount(), loader.GetVertexSize(), loader.GetVertices());
  indexBuffer->Create(&(device->GetDevice()), loader.GetIndexSize() * loader.GetIndexCount(), loader.GetIndices(), DXGI_FORMAT_R32_UINT);
  index_count_ = 321567;

  // 定数バッファを作成
  constantBufferView = device->CreateConstantBuffer(sizeof(ConstantBufferType));

  {
    // シェーダーリソースバッファを作成
    shaderResourceBuffer = device->CreateShaderResourceBuffer(INSTANCE_NUM, sizeof(InstanceBuffer));

    // インスタンス個別のデータを初期化
    instanceBufferData.resize(INSTANCE_NUM);
    DirectX::XMStoreFloat4x4(&(instanceBufferData[0].world), DirectX::XMMatrixIdentity());
    DirectX::XMStoreFloat4x4(&(instanceBufferData[1].world), DirectX::XMMatrixTranslation(1.0f, 1.0f, 1.0f));
    DirectX::XMStoreFloat4x4(&(instanceBufferData[2].world), DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationZ(DirectX::XM_PI), DirectX::XMMatrixTranslation(2.0f, 2.0f, 2.0f)));
    DirectX::XMStoreFloat4x4(&(instanceBufferData[3].world), DirectX::XMMatrixTranslation(-1.0f, -1.0f, -1.0f));
    DirectX::XMStoreFloat4x4(&(instanceBufferData[4].world), DirectX::XMMatrixTranslation(-2.0f, -2.0f, -2.0f));
    shaderResourceBuffer->Map(&instanceBufferData[0], sizeof(InstanceBuffer) * instanceBufferData.size());
  }

  QImage image;

  // ベースカラー(アルベド)
  if (image.load("../Resources/Texture/marble-speckled-albedo.png"))
  {
    auto format = image.format();
    auto bytesPerLine = image.bytesPerLine();
    auto width = image.width();
    auto height = image.height();
    auto buffer = std::make_unique<uchar[]>(bytesPerLine * height);
    std::memcpy(buffer.get(), image.convertToFormat(QImage::Format::Format_RGBA8888).constBits(), bytesPerLine * height);

    // テクスチャバッファを作成
    device->CreateTextureBuffer(buffer.get(), width, height, 4);

    buffer.reset(nullptr);
  }

  // メタリック(金属性)
  if (image.load("../Resources/Texture/marble-speckled-metalness.png"))
  {
    auto format = image.format();
    auto bytesPerLine = image.bytesPerLine();
    auto width = image.width();
    auto height = image.height();
    auto buffer = std::make_unique<uchar[]>(bytesPerLine * height);
    std::memcpy(buffer.get(), image.convertToFormat(QImage::Format::Format_RGBA8888).constBits(), bytesPerLine * height);

    // テクスチャバッファを作成
    device->CreateTextureBuffer(buffer.get(), width, height, 4);

    buffer.reset(nullptr);
  }

  // ラフネス(粗さ)
  if (image.load("../Resources/Texture/marble-speckled-roughness.png"))
  {
    auto format = image.format();
    auto bytesPerLine = image.bytesPerLine();
    auto width = image.width();
    auto height = image.height();
    auto buffer = std::make_unique<uchar[]>(bytesPerLine * height);
    std::memcpy(buffer.get(), image.convertToFormat(QImage::Format::Format_RGBA8888).constBits(), bytesPerLine * height);

    // テクスチャバッファを作成
    device->CreateTextureBuffer(buffer.get(), width, height, 4);

    buffer.reset(nullptr);
  }

  // プリミティブタイプの設定
  topology_ = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

  // 非同期ジョブの準備
  App::job_system::AsyncJobManager::GetInstance()->Create();

  // レベルの作成
  level_ = std::make_unique<App::actor::ViewerLevel>();
  level_->SetDevice(std::move(device));
  level_->Create();

  is_loading = false;

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
  if (is_loading)
  {
    return;
  }

  // デバッグ用FPS表示
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

  // 定数バッファを更新
  {
    // ワールド行列を更新
    DirectX::XMStoreFloat4x4(&(constantBuffer.world), DirectX::XMMatrixRotationQuaternion(level_->GetModelQuaternion()));

    // ジョブ実行
    App::job_system::JobScheduler::GetInstance()->Execute(6);

    // ビュー行列を作成
    DirectX::XMVECTOR eye = level_->GetCameraPosition();
    DirectX::XMVECTOR at = level_->GetLookAt();
    DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
    DirectX::XMStoreFloat4x4(&(constantBuffer.view), DirectX::XMMatrixLookAtLH(eye, at, up));

    // プロジェクション行列を作成
    DirectX::XMStoreFloat4x4(&(constantBuffer.projection), DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 3.0f, 600.0f / 400.0f, 0.1f, 1000.0f));

    // 定数バッファを更新
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
  // TODO:ウィンドウのリサイズに対応する
  QSize size = event->size();
}

void QtDirect3D12::dragEnterEvent(QDragEnterEvent *event)
{
    // ファイル形式のみ受け付ける
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void QtDirect3D12::dropEvent(QDropEvent* event)
{
  // 読み込み中ダイアログを表示
  QProgressDialog progress(
    u8"しばらくお待ちください・・・",
    u8"キャンセル",
    0,
    0,
    this,
    0
  );

  progress.setFixedSize(progress.sizeHint());     // ウィンドウをリサイズできないようにする
  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle(u8"処理中");
  progress.show();
  progress.setValue(100);

  level_->RegisterLoadedEvent([&](App::IAppModel* model) {

    decltype(auto) device = level_->GetDevice();
    decltype(auto) vertices = model->GetVertices();
    decltype(auto) indices = model->GetIndices();

    // 頂点データ、インデックスバッファの更新
    auto vertex_size = sizeof(App::IAppModel::Vertex);
    vertexBuffer->Create(&(device.GetDevice()), vertex_size * vertices.size(), vertex_size, &(vertices[0]));
    indexBuffer->Create(&(device.GetDevice()), sizeof(uint32_t) * indices.size(), &(indices[0]), DXGI_FORMAT_R32_UINT);
    index_count_ = indices.size();

    is_loading = false;
  });

  is_loading = true;

  // 非同期読み込み
  QString file = event->mimeData()->urls().first().toLocalFile();
  level_->LoadModel(file.toLocal8Bit().toStdString());
}

void QtDirect3D12::wheelEvent(QWheelEvent* event)
{
  // 注視点との距離

  QPoint degrees = event->angleDelta() / 8;
  const auto force = degrees.y() / std::abs(degrees.y());

  // レベルにホイール回転イベントを通知する
  level_->DispatchWheelEvent(force);

  // TODO:滑らかに移動するようにする(ジョブに登録して移動させる?)
  // 現在の視点と移動先の視点を線形補間で指定時間で移動を行うようにする
  // 移動中に移動先の視点が更新される可能性を考慮する
  // メインループのジョブ実行順序は
  // 1.ファイル読み込み(非同期ジョブ)
  // 2.座標更新
  // 3.描画にする
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
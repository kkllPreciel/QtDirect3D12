// TODO:includeの順序をコーディングルールに合わせる
#include <qevent.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <Sein\Direct3D12\constant_buffer.h>
#include <Sein\Direct3D12\shader_resource_buffer.h>
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
  model_(nullptr),
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

  model_ = std::move(App::IAppModel::LoadFromPmx(*(device.get()), "../Resources/Pmx/YYB式桜ミクv1.00.pmx"));

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

  // ベースカラー(アルベド)のテクスチャバッファを作成
  device->CreateTextureBufferFromFile(L"../Resources/Texture/marble-speckled-albedo.png");

  // メタリック(金属性)のテクスチャバッファを作成
  device->CreateTextureBufferFromFile(L"../Resources/Texture/marble-speckled-metalness.png");

  // ラフネス(粗さ)のテクスチャバッファを作成
  device->CreateTextureBufferFromFile(L"../Resources/Texture/marble-speckled-roughness.png");

  // プリミティブタイプの設定
  topology_ = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

  // 非同期ジョブの準備
  App::job_system::AsyncJobManager::GetInstance()->Create();

  // ジョブスケジューラの作成
  App::job_system::JobScheduler::GetInstance()->Create(std::thread::hardware_concurrency() - 1);

  // レベルの作成
  level_ = std::make_unique<App::actor::ViewerLevel>();
  level_->SetDevice(std::move(device));
  level_->Create();

  // 読み込み終了シグナルにスロット(関数)を登録する
  level_->RegisterLoadedSignalEvent([&](App::IAppModel* model) {
    is_loading = false;
  });

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

  // イベントの処理
  {
    // レベルにホイール回転イベントを通知する
    if (direction_)
    {
      level_->OnMouseWheelEvent(direction_);
      direction_ = 0;
    }
  }

  // 定数バッファを更新
  {
    // ジョブ実行
    App::job_system::JobScheduler::GetInstance()->Execute(6);

    // ビュー行列を作成
    DirectX::XMStoreFloat4x4(&(constantBuffer.view), level_->GetViewMatrix());

    // プロジェクション行列を作成
    DirectX::XMStoreFloat4x4(&(constantBuffer.projection), DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 3.0f, 600.0f / 400.0f, 0.1f, 1000.0f));

    // 定数バッファを更新
    constantBufferView->Map(&constantBuffer, sizeof(ConstantBufferType));
  }

  decltype(auto) device = level_->GetDevice();
  device.BeginScene();
  device.SetPrimitiveTopology(topology_);
  device.SetVertexBuffers(0, 1, &(model_->GetVertexBuffer().GetView()));
  device.SetIndexBuffer(&(model_->GetIndexBuffer().GetView()));
  device.Render(model_->GetIndices().size(), 1);
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

  is_loading = true;

  // 非同期読み込み
  QString file = event->mimeData()->urls().first().toLocalFile();
  level_->LoadModel(file.toLocal8Bit().toStdString());
}

void QtDirect3D12::wheelEvent(QWheelEvent* event)
{
  // 注視点との距離

  QPoint degrees = event->angleDelta() / 8;
  direction_ = degrees.y() / std::abs(degrees.y());

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

void QtDirect3D12::mousePressEvent(QMouseEvent *event)
{
  // 左クリックのみ処理する
  if (event->button() != Qt::LeftButton)
  {
    return;
  }

  auto x = static_cast<float>(event->x());
  auto y = static_cast<float>(event->y());

  // レベルにマウスプレスイベントを発行する
  level_->OnMousePressEvent({ x, y, 0 });
}

void QtDirect3D12::mouseReleaseEvent(QMouseEvent *event)
{
  // 左クリックのみ処理する
  if (event->button() != Qt::LeftButton)
  {
    return;
  }

  auto x = static_cast<float>(event->x());
  auto y = static_cast<float>(event->y());

  // レベルにマウスリリースイベントを発行する
  level_->OnMouseReleaseEvent({ x, y, 0 });
}

void QtDirect3D12::mouseMoveEvent(QMouseEvent *event)
{
  auto x = static_cast<float>(event->x());
  auto y = static_cast<float>(event->y());

  // レベルにマウス移動イベントを発行する
  level_->OnMouseMoveEvent({ x, y, 0 });
}
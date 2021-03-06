/**
 *  @file     viewer_level.cc
 *  @brief    ビューア用レベルに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/02/06
 *  @version  1.0
 */

 // include
#include "actor/viewer_level.h"
#include "job_system/job_scheduler.h"
#include "job_system/async_job_manager.h"
#include "actor/model_render_component.h"
#include "actor/camera_component.h"
#include "actor/mouse_driven_camera_move_component.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  コンストラクタ
     */
    ViewerLevel::ViewerLevel()
    {

    }

    /**
     *  @brief  デストラクタ
     */
    ViewerLevel::~ViewerLevel()
    {
      Destroy();
    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool ViewerLevel::Create()
    {
      // アクターを生成する(TODO:設定ファイルから読み込む)
      {
        auto camera_component = actors_[0].AddComponent<actor::CameraComponent>();
        camera_component->SetLookAt({ 0.0f, 10.0f, 0.0f });
        camera_component->SetUpDirection({ 0.0f, 1.0f, 0.0f });
        actors_[0].SetPosition({ 0.0f, 10.0f, -30.5f });

        // マウスで操作用コンポーネントを作成
        auto component = actors_[0].AddComponent<actor::MouseDrivenCameraMoveComponent>();
        component->Create();
        component->SetTime(1);
        component->SetVelocity(1.0f);
        component->SetCoefficient(0.5f);
        component->SetDistance(1.0f);
      }

      // モデル用アクターを生成する
      actors_[1].SetPosition({ 0.0f, 10.0f, 0.0f });

      // モデル描画用コンポーネントを作成
      decltype(auto) model_render_component = actors_[1].AddComponent<actor::ModelRenderComponent>();
      model_render_component->SetTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      return true;
    }

    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void ViewerLevel::Execute(std::uint64_t delta_time)
    {
      if (initialized_ == false && async_job_.IsFinished())
      {
        initialized_ = true;
      }
      else if (async_job_.IsFinished())
      {
        // 更新処理を行う
      }
    }

    /**
     *  @brief  終了処理を行う
     */
    void ViewerLevel::Destroy()
    {
      for (auto& actor : actors_)
      {
        actor.Destroy();
      }

      loaded_.disconnect_all_slots();
    }

    /**
     *  @brief  モデルを読み込む
     *  @param  file_path:読み込むモデルのファイルパス
     */
    void ViewerLevel::LoadModel(std::string file_path)
    {
      file_path_ = file_path;

      // job_.SetFunction([&](std::uint64_t delta_time) { this->Execute(delta_time); });
      // job_system::JobScheduler::GetInstance()->Register(&job_, job_system::JobScheduler::kMoveUpdate);

      // 非同期関数のジョブ登録
      async_job_.SetFunction([&]() { this->AsyncExecute(); });
      job_system::AsyncJobManager::GetInstance()->Push(&async_job_);
    }

    /**
     *  @brief  読み込み終了シグナルイベントにスロット(関数)を登録する
     *  @param  callback:読み込み終了時に実行するスロット(関数)
     */
    void ViewerLevel::RegisterLoadedSignalEvent(std::function<void(App::IAppModel*)> callback)
    {
      loaded_.connect(callback);
    }

    /**
     *  @brief  マウスクリックイベントが発生した
     *  @param  position:マウスの座標
     */
    void ViewerLevel::OnMousePressEvent(DirectX::XMVECTOR position)
    {
      // TODO:オブサーバーパターンを使用して実装する
      actors_[0].GetComponent<actor::MouseDrivenCameraMoveComponent>()->OnMousePressEvent(position);
    }
    
    /**
     *  @brief  マウスリリースイベントが発生した
     *  @param  position:マウスの座標
     */
    void ViewerLevel::OnMouseReleaseEvent(DirectX::XMVECTOR position)
    {
      // TODO:オブサーバーパターンを使用して実装する
      actors_[0].GetComponent<actor::MouseDrivenCameraMoveComponent>()->OnMouseReleaseEvent(position);
    }
    
    /**
     *  @brief  マウス移動イベントが発生した
     *  @param  position:マウスの座標
     */
    void ViewerLevel::OnMouseMoveEvent(DirectX::XMVECTOR position)
    {
      // TODO:オブサーバーパターンを使用して実装する
      actors_[0].GetComponent<actor::MouseDrivenCameraMoveComponent>()->OnMouseMoveEvent(position);
    }
    
    /**
     *  @brief  マウスホイールイベントが発生した
     *  @param  direction:ホイールの回転方向
     */
    void ViewerLevel::OnMouseWheelEvent(std::int32_t direction)
    {
      // TODO:オブサーバーパターンを使用して実装する
      actors_[0].GetComponent<actor::MouseDrivenCameraMoveComponent>()->OnMouseWheelEvent(direction);
    }

    /**
     *  @brief  ビュー行列を取得する
     *  @return ビュー行列
     */
    const DirectX::XMMATRIX& ViewerLevel::GetViewMatrix()
    {
      return actors_[0].GetComponent<actor::CameraComponent>()->GetViewMatrix();
    }

    /**
     *  @brief  デバイスを設定する
     *  @param  device:デバイス
     */
    void ViewerLevel::SetDevice(std::unique_ptr<Sein::Direct3D12::Device> device)
    {
      device_.reset(nullptr);
      device_ = std::move(device);
    }

    /**
     *  @brief  デバイスを取得する
     *  @return デバイス
     */
    Sein::Direct3D12::Device& ViewerLevel::GetDevice()
    {
      return *device_;
    }

    /**
     *  @brief  非同期実行処理関数
     */
    void ViewerLevel::AsyncExecute()
    {
      // 非同期読み込み処理

      auto model = App::IAppModel::LoadFromObj(*device_, file_path_);
      loaded_(model.get());
    }
  };
};
/**
 *  @file     mouse_driven_camera_move_component.cc
 *  @brief    マウスで操作できるカメラ移動用コンポーネントに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/02/22
 *  @version  1.0
 */

 // include
#include "actor/mouse_driven_camera_move_component.h"
#include "actor/camera_component.h"
#include "job_system/job_scheduler.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  引数付きコンストラクタ
     *  @param  owner:コンポーネントを所持しているオーナー
     */
    MouseDrivenCameraMoveComponent::MouseDrivenCameraMoveComponent(Actor* owner) : Component(owner)
    {

    }

    /**
     *  @brief  デストラクタ
     */
    MouseDrivenCameraMoveComponent::~MouseDrivenCameraMoveComponent()
    {
      Destroy();
    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool MouseDrivenCameraMoveComponent::Create()
    {
      clicked_ = false;

      Destroy();

      return true;
    }

    /**
     *  @brief  終了処理を行う
     */
    void MouseDrivenCameraMoveComponent::Destroy()
    {
      // ジョブスケジューラへの登録を解除する
      if (job_)
      {
        job_system::JobScheduler::GetInstance()->Unregister(job_.get());
      }

      job_.reset();

      delta_ = 0;
    }

    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void MouseDrivenCameraMoveComponent::Update(std::uint64_t delta_time)
    {
      delta_ += delta_time;
      auto time = static_cast<float>(delta_) / static_cast<float>(time_ * 60);

      DirectX::XMVECTOR now = DirectX::XMVectorLerp(begin_position_, end_position_, time);
      //now = DirectX::XMVectorAdd(DirectX::XMVectorScale(begin_position_, (1.0f - time_)), DirectX::XMVectorScale(end_position_, time_));
      owner_->SetPosition(now);

      // カメラのダーティ(更新)フラグを立てる
      owner_->GetComponent<CameraComponent>()->SetDirty(true);

      // 移動が終了したら自動的に削除する
      if (time_ * 60 <= delta_)
      {
        Destroy();
      }
    }

    /**
     *  @brief  移動速度を設定する
     *  @param  velocity:移動速度
     */
    void MouseDrivenCameraMoveComponent::SetVelocity(const float velocity)
    {
      velocity_ = velocity;
    }
    
    /**
     *  @brief  移動係数を設定する
     *  @param  coefficient:移動係数
     */
    void MouseDrivenCameraMoveComponent::SetCoefficient(const float coefficient)
    {
      coefficient_ = coefficient;
    }

    /**
     *  @brief  移動に必要な時間を設定する
     *  @param  time:移動に必要な時間
     */
    void MouseDrivenCameraMoveComponent::SetTime(std::uint64_t time)
    {
      time_ = time;
    }

    /**
     *  @brief  注視点との最低距離を設定する
     *  @param  distance:注視点との最低距離
     */
    void MouseDrivenCameraMoveComponent::SetDistance(const float distance)
    {
      assert(1.0f <= distance);
      distance_ = distance;
    }
    
    /**
     *  @brief  マウスクリックイベントが発生した
     *  @param  position:マウスの座標
     */
    void MouseDrivenCameraMoveComponent::OnMousePressEvent(DirectX::XMVECTOR position)
    {
      Destroy();

      prev_mouse_position_ = position;
      clicked_ = true;
    }
    
    /**
     *  @brief  マウスリリースイベントが発生した
     *  @param  position:マウスの座標
     */
    void MouseDrivenCameraMoveComponent::OnMouseReleaseEvent(DirectX::XMVECTOR position)
    {
      clicked_ = false;
    }
    
    /**
     *  @brief  マウス移動イベントが発生した
     *  @param  position:マウスの座標
     */
    void MouseDrivenCameraMoveComponent::OnMouseMoveEvent(DirectX::XMVECTOR position)
    {
      if (clicked_ == false)
      {
        return;
      }

      // カメラから注視点へのベクトルは分かる
      // カメラの上方向ベクトルも分かる
      // カメラの横方向ベクトルも計算可能
      // マウスの移動ベクトルに応じて上方向ベクトルと横方向ベクトルを合算する

      auto eye = owner_->GetPosition();

      auto component = owner_->GetComponent<CameraComponent>();
      auto at = component->GetLookAt();
      auto up = component->GetUpDirection();
      auto cross = DirectX::XMVector3Cross(up, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(at, eye)));

      auto value = DirectX::XMVectorSubtract(position, prev_mouse_position_);

      // 移動方向ベクトル(正規化済み)を作成
      auto dir = DirectX::XMVector3Normalize(DirectX::XMVectorAdd(DirectX::XMVectorScale(up, -value.m128_f32[1]), DirectX::XMVectorScale(cross, value.m128_f32[0])));

      // 回転軸を作成
      auto rotation_axis = DirectX::XMQuaternionRotationAxis(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(at, eye)), dir), 0.1);

      // カメラの回転を行う
      owner_->SetPosition(DirectX::XMVector3Rotate(eye, rotation_axis));
      component->SetUpDirection(DirectX::XMVector3Rotate(up, rotation_axis));

      prev_mouse_position_ = position;

      // カメラのダーティ(更新)フラグを立てる
      component->SetDirty(true);
    }
    
    /**
     *  @brief  マウスホイールイベントが発生した
     *  @param  direction:ホイールの回転方向
     */
    void MouseDrivenCameraMoveComponent::OnMouseWheelEvent(std::int32_t direction)
    {
      Destroy();

      if (clicked_)
      {
        return;
      }

      // 視点から注視点へのベクトルを作成
      DirectX::XMVECTOR eye = owner_->GetPosition();
      DirectX::XMVECTOR at = owner_->GetComponent<CameraComponent>()->GetLookAt();
      DirectX::XMVECTOR dir = DirectX::XMVectorSubtract(at, eye);

      // 視点から注視点への距離を取得
      float distance = 0.0f;
      DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(dir));

      // 距離が離れている場合は移動量に補正を行う
      const auto move = velocity_ * (std::fabsf(distance) * coefficient_);

      DirectX::XMVECTOR target = DirectX::XMVectorAdd(eye, DirectX::XMVectorScale(DirectX::XMVector3Normalize(dir), direction * move));

      // 注視点との距離が一定以下なら近づけないようにする
      dir = DirectX::XMVectorSubtract(at, target);
      distance = 0.0f;
      DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(dir));
      if (distance < distance_)
      {
        return;
      }

      begin_position_ = eye;
      end_position_ = target;

      // ジョブに登録する
      job_ = std::make_unique<job_system::Job>();

      job_->SetFunction([&](std::uint64_t delta_time) {
        this->Update(delta_time);
      });
      job_system::JobScheduler::GetInstance()->Register(job_.get(), job_system::JobScheduler::kMoveUpdate);
    }
  };
};
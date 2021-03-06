/**
 *  @file     camera_move_component.cc
 *  @brief    カメラ移動処理用コンポーネントに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/22
 *  @version  1.0
 */

 // include
#include "actor/camera_move_component.h"
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
    CameraMoveComponent::CameraMoveComponent(Actor* owner) : Component(owner)
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    CameraMoveComponent::~CameraMoveComponent()
    {
      Destroy();
    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool CameraMoveComponent::Create()
    {
      Destroy();

      return true;
    }

    /**
     *  @brief  終了処理を行う
     */
    void CameraMoveComponent::Destroy()
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
     *  @brief  発火する
     *  @param  direction:方向
     */
    void CameraMoveComponent::Ignition(const float direction)
    {
      Destroy();

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

    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void CameraMoveComponent::Update(std::uint64_t delta_time)
    {
      delta_ += delta_time;
      auto time = static_cast<float>(delta_) / static_cast<float>(time_ * 60);

      DirectX::XMVECTOR now = DirectX::XMVectorLerp(begin_position_, end_position_, time);
      //now = DirectX::XMVectorAdd(DirectX::XMVectorScale(begin_position_, (1.0f - time_)), DirectX::XMVectorScale(end_position_, time_));
      owner_->SetPosition(now);

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
    void CameraMoveComponent::SetVelocity(const float velocity)
    {
      velocity_ = velocity;
    }
    
    /**
     *  @brief  移動係数を設定する
     *  @param  coefficient:移動係数
     */
    void CameraMoveComponent::SetCoefficient(const float coefficient)
    {
      coefficient_ = coefficient;
    }

    /**
     *  @brief  移動に必要な時間を設定する
     *  @param  time:移動に必要な時間
     */
    void CameraMoveComponent::SetTime(std::uint64_t time)
    {
      time_ = time;
    }

    /**
     *  @brief  注視点との最低距離を設定する
     *  @param  distance:注視点との最低距離
     */
    void CameraMoveComponent::SetDistance(const float distance)
    {
      assert(1.0f <= distance);
      distance_ = distance;
    }
  };
};
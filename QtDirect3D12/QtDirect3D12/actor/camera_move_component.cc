/**
 *  @file     camera_move_component.cc
 *  @brief    カメラ移動処理用コンポーネントに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/22
 *  @version  1.0
 */

 // include
#include "actor/camera_move_component.h"
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

    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool CameraMoveComponent::Create()
    {
      Destroy();

      job_ = std::make_unique<job_system::Job>();

      job_->SetFunction([&](std::uint64_t delta_time) {
        this->Update(delta_time);
      });
      job_system::JobScheduler::GetInstance()->Register(job_.get(), job_system::JobScheduler::kMoveUpdate);

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
     *  @brief  初期座標を設定する
     *  @param  begin_position:初期座標
     */
    void CameraMoveComponent::SetBeginPosition(DirectX::XMVECTOR begin_position)
    {
      begin_position_ = begin_position;
    }

    /**
     *  @brief  目標座標を設定する
     *  @param  end_position:目標座標
     */
    void CameraMoveComponent::SetEndPosition(DirectX::XMVECTOR end_position)
    {
      end_position_ = end_position;
    }

    /**
     *  @brief  移動に必要な時間を設定する
     *  @param  time:移動に必要な時間
     */
    void CameraMoveComponent::SetTime(std::uint64_t time)
    {
      time_ = time;
    }
  };
};
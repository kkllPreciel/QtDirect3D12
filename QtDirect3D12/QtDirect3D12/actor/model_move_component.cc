/**
 *  @file     model_move_component.cc
 *  @brief    モデル移動処理用コンポーネントに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/02/08
 *  @version  1.0
 */

 // include
#include "actor/model_move_component.h"
#include "job_system/job_scheduler.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  引数付きコンストラクタ
     *  @param  owner:コンポーネントを所持しているオーナー
     */
    ModelMoveComponent::ModelMoveComponent(Actor* owner) : Component(owner)
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    ModelMoveComponent::~ModelMoveComponent()
    {
      Destroy();
    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool ModelMoveComponent::Create()
    {
      Destroy();

      // ジョブに登録する
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
    void ModelMoveComponent::Destroy()
    {
      // ジョブスケジューラへの登録を解除する
      if (job_)
      {
        job_system::JobScheduler::GetInstance()->Unregister(job_.get());
      }

      job_.reset();
    }

    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void ModelMoveComponent::Update(std::uint64_t delta_time)
    {
      auto axis = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0, 1, 0));

      owner_->SetRotation(DirectX::XMQuaternionMultiply(owner_->GetRotation(), DirectX::XMQuaternionRotationRollPitchYaw(0, angle_, 0)));
    }
    
    /**
     *  @brief  1フレームに回転する角度(ラジアン)を設定する
     *  @param  angle:1フレームに回転する角度
     */
    void ModelMoveComponent::SetRotationAngle(const float angle)
    {
      angle_ = angle;
    }
  };
};
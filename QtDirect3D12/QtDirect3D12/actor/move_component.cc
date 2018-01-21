/**
 *  @file     move_component.cc
 *  @brief    移動処理用コンポーネントに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/21
 *  @version  1.0
 */

 // include
#include "actor/move_component.h"
#include "actor/actor.h"
#include "job_system/job_scheduler.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  引数付きコンストラクタ
     *  @param  owner:コンポーネントを所持しているオーナー
     */
    MoveComponent::MoveComponent(Actor* owner) : Component(owner)
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    MoveComponent::~MoveComponent()
    {

    }
    
    /**
     *  @brief  コンポーネントの種別IDを取得する
     *  @return コンポーネントの種別ID
     */
    constexpr Component::ID MoveComponent::GetId()
    {
      return Component::ID::kMove;
    }
    

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool MoveComponent::Create()
    {
      // TODO:初期化処理

      return true;
    }
    
    /**
     *  @brief  終了処理を行う
     */
    void MoveComponent::Destroy()
    {

    }

    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void MoveComponent::Update(std::uint64_t delta_time)
    {
      // TODO:更新処理
    }
    
    /**
     *  @brief  移動方向を設定する
     *  @param  velocity:移動方向
     */
    void MoveComponent::SetVelocity(DirectX::XMVECTOR velocity)
    {
      velocity_ = velocity;
    }
    
    /**
     *  @brief  移動方向を取得する
     *  @return 移動方向
     */
    DirectX::XMVECTOR MoveComponent::GetVelocity() const
    {
      return velocity_;
    }
    
    /**
     *  @brief  速度を設定する
     *  @param  speed:速度
     */
    void MoveComponent::SetSpeed(float speed)
    {
      speed_ = speed;
    }
    
    /**
     *  @brief  速度を取得する
     *  @return 速度
     */
    float MoveComponent::GetSpeed() const
    {
      return speed_;
    }
  };
};
/**
 *  @file     actor.cc
 *  @brief    アクターに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/20
 *  @version  1.0
 */

 // include
#include "actor/actor.h"
#include <cassert>
#include "actor/component.h"
#include "job_system/job.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  コンストラクタ
     */
    Actor::Actor()
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    Actor::~Actor()
    {
      Destroy();
    }
    
    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool Actor::Create()
    {
      return true;
    }
    
    /**
     *  @brief  終了処理を行う
     */
    void Actor::Destroy()
    {
      for (decltype(auto) component : components_)
      {
        component.second->Destroy();
        delete component.second;
      }

      components_.clear();
    }
    
    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void Actor::Update(std::uint64_t delta_time)
    {

    }
    
    /**
     *  @brief  ユニークIDを設定する
     *  @param  unique_id:ユニークID
     */
    void Actor::SetUniqueId(std::uint64_t unique_id)
    {
      unique_id_ = unique_id;
    }
    
    /**
     *  @brief  ユニークIDを取得する
     *  @return ユニークID
     */
    std::uint64_t Actor::GetUniqueId() const
    {
      return unique_id_;
    }
    
    /**
     *  @brief  座標を設定する
     *  @param  position:設定する座標
     */
    void Actor::SetPosition(DirectX::XMVECTOR position)
    {
      position_ = position;
    }

    /**
     *  @brief  座標を取得する
     *  @return 座標
     */
    DirectX::XMVECTOR Actor::GetPosition() const
    {
      return position_;
    }
    
    /**
     *  @brief  回転を設定する
     *  @param  rotation:設定する回転
     */
    void Actor::SetRotation(DirectX::XMVECTOR rotation)
    {
      rotation_ = rotation;
    }

    /**
     *  @brief  回転を取得する
     *  @return 回転
     */
    DirectX::XMVECTOR Actor::GetRotation() const
    {
      return rotation_;
    }
  };
};
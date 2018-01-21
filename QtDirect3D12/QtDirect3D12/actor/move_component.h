/**
 *  @file     move_component.h
 *  @brief    移動処理用コンポーネントに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/21
 *  @version  1.0
 */

#pragma once

 // include
#include <DirectXMath.h>
#include "actor/component.h"
#include "job_system/job.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  移動処理用のコンポーネントクラス
     */
    class MoveComponent : public Component
    {
    public:
      /**
       *  @brief  引数付きコンストラクタ
       *  @param  owner:コンポーネントを所持しているオーナー
       */
      MoveComponent(Actor* owner);

      /**
       *  @brief  デストラクタ
       */
      virtual ~MoveComponent();

      /**
       *  @brief  コンストラクタ
       */
      MoveComponent() = delete;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      MoveComponent(const MoveComponent& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      MoveComponent& operator = (const MoveComponent& other) = delete;

      /**
       *  @brief  コンポーネントの種別IDを取得する
       *  @return コンポーネントの種別ID
       */
      static constexpr Component::ID GetId();

      /**
       *  @brief  初期化を行う
       *  @return 初期化成功フラグ
       */
      virtual bool Create() override;

      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() override;

      /**
       *  @brief  更新処理を行う
       *  @param  delta_time:1フレームの経過時間
       */
      void Update(std::uint64_t delta_time);

      /**
       *  @brief  移動方向を設定する
       *  @param  velocity:移動方向
       */
      void SetVelocity(DirectX::XMVECTOR velocity);

      /**
       *  @brief  移動方向を取得する
       *  @return 移動方向
       */
      DirectX::XMVECTOR GetVelocity() const;

      /**
       *  @brief  速度を設定する
       *  @param  speed:速度
       */
      void SetSpeed(float speed);

      /**
       *  @brief  速度を取得する
       *  @return 速度
       */
      float GetSpeed() const;

    private:
      DirectX::XMVECTOR velocity_ = DirectX::XMVectorZero();  ///< 移動方向
      float speed_ = 0.0f;                                    ///< 速度
    };
  };
};
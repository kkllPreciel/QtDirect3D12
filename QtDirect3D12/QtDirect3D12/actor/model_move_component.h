/**
 *  @file     model_move_component.h
 *  @brief    モデル移動処理用コンポーネントに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/02/08
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <DirectXMath.h>
#include "actor/component.h"
#include "job_system/job.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  モデル移動処理用のコンポーネントクラス
     */
    class ModelMoveComponent : public Component
    {
    public:
      /**
       *  @brief  引数付きコンストラクタ
       *  @param  owner:コンポーネントを所持しているオーナー
       */
      ModelMoveComponent(Actor* owner);

      /**
       *  @brief  デストラクタ
       */
      virtual ~ModelMoveComponent();

      /**
       *  @brief  コンストラクタ
       */
      ModelMoveComponent() = delete;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      ModelMoveComponent(const ModelMoveComponent& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      ModelMoveComponent& operator = (const ModelMoveComponent& other) = delete;

      /**
       *  @brief  コンポーネントの種別IDを取得する
       *  @return コンポーネントの種別ID
       */
      static constexpr std::uint32_t GetId()
      {
        return Component::ID::kMove;
      }

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
       *  @brief  1フレームに回転する角度(ラジアン)を設定する
       *  @param  angle:1フレームに回転する角度
       */
      void SetRotationAngle(const float angle);

    private:
      std::unique_ptr<job_system::Job> job_;  ///< ジョブ
      float angle_;                           ///< 1フレームに回転する角度
    };
  };
};
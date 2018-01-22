/**
 *  @file     camera_move_component.h
 *  @brief    カメラ移動処理用コンポーネントに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/22
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
     *  @brief  カメラ移動処理用のコンポーネントクラス
     */
    class CameraMoveComponent : public Component
    {
    public:
      /**
       *  @brief  引数付きコンストラクタ
       *  @param  owner:コンポーネントを所持しているオーナー
       */
      CameraMoveComponent(Actor* owner);

      /**
       *  @brief  デストラクタ
       */
      virtual ~CameraMoveComponent();

      /**
       *  @brief  コンストラクタ
       */
      CameraMoveComponent() = delete;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      CameraMoveComponent(const CameraMoveComponent& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      CameraMoveComponent& operator = (const CameraMoveComponent& other) = delete;

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
       *  @brief  初期座標を設定する
       *  @param  begin_position:初期座標
       */
      void SetBeginPosition(DirectX::XMVECTOR begin_position);

      /**
       *  @brief  目標座標を設定する
       *  @param  end_position:目標座標
       */
      void SetEndPosition(DirectX::XMVECTOR end_position);

      /**
       *  @brief  移動に必要な時間を設定する
       *  @param  time:移動に必要な時間
       */
      void SetTime(std::uint64_t time);

    private:
      DirectX::XMVECTOR begin_position_;      ///< 初期座標
      DirectX::XMVECTOR end_position_;        ///< 目標座標
      std::uint64_t time_;                    ///< 移動に必要な時間
      std::uint64_t delta_;                   ///< 現在の経過時間
      std::unique_ptr<job_system::Job> job_;  ///< ジョブ
    };
  };
};
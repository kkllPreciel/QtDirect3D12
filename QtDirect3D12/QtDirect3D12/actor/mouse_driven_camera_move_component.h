/**
 *  @file     mouse_driven_camera_move_component.h
 *  @brief    マウスで操作できるカメラ移動用コンポーネントに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/02/22
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
     *  @brief  マウスで操作できるカメラ移動用のコンポーネントクラス
     */
    class MouseDrivenCameraMoveComponent : public Component
    {
    public:
      /**
       *  @brief  引数付きコンストラクタ
       *  @param  owner:コンポーネントを所持しているオーナー
       */
      MouseDrivenCameraMoveComponent(Actor* owner);

      /**
       *  @brief  デストラクタ
       */
      virtual ~MouseDrivenCameraMoveComponent();

      /**
       *  @brief  コンストラクタ
       */
      MouseDrivenCameraMoveComponent() = delete;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      MouseDrivenCameraMoveComponent(const MouseDrivenCameraMoveComponent& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      MouseDrivenCameraMoveComponent& operator = (const MouseDrivenCameraMoveComponent& other) = delete;

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
       *  @brief  移動速度を設定する
       *  @param  velocity:移動速度
       */
      void SetVelocity(const float velocity);

      /**
       *  @brief  移動係数を設定する
       *  @param  coefficient:移動係数
       */
      void SetCoefficient(const float coefficient);

      /**
       *  @brief  注視点との最低距離を設定する
       *  @param  distance:注視点との最低距離
       */
      void SetDistance(const float distance);

      /**
       *  @brief  移動に必要な時間を設定する
       *  @param  time:移動に必要な時間
       */
      void SetTime(std::uint64_t time);

      /**
       *  @brief  マウスクリックイベントが発生した
       *  @param  position:マウスの座標
       */
      void OnMousePressEvent(DirectX::XMVECTOR position);

      /**
       *  @brief  マウスリリースイベントが発生した
       *  @param  position:マウスの座標
       */
      void OnMouseReleaseEvent(DirectX::XMVECTOR position);

      /**
       *  @brief  マウス移動イベントが発生した
       *  @param  position:マウスの座標
       */
      void OnMouseMoveEvent(DirectX::XMVECTOR position);

      /**
       *  @brief  マウスホイールイベントが発生した
       *  @param  direction:ホイールの回転方向
       */
      void OnMouseWheelEvent(std::int32_t direction);

    private:
      std::unique_ptr<job_system::Job> job_;  ///< ジョブ
      float velocity_;                        ///< 移動速度
      float coefficient_;                     ///< 移動係数
      float distance_;                        ///< 注視点との最低距離
      DirectX::XMVECTOR begin_position_;      ///< 初期座標
      DirectX::XMVECTOR end_position_;        ///< 目標座標
      std::uint64_t time_;                    ///< 移動に必要な時間
      std::uint64_t delta_;                   ///< 現在の経過時間
      DirectX::XMVECTOR prev_mouse_position_; ///< 1つ前のマウスの座標
      bool clicked_;                          ///< クリックフラグ
    };
  };
};
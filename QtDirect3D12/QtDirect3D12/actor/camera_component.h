/**
 *  @file     camera_component.h
 *  @brief    カメラ用コンポーネントに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/27
 *  @version  1.0
 */

#pragma once

 // include
#include <DirectXMath.h>
#include "actor/component.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  カメラ用のコンポーネントクラス
     */
    class CameraComponent : public Component
    {
    public:
      /**
       *  @brief  引数付きコンストラクタ
       *  @param  owner:コンポーネントを所持しているオーナー
       */
      CameraComponent(Actor* owner);

      /**
       *  @brief  デストラクタ
       */
      virtual ~CameraComponent();

      /**
       *  @brief  コンストラクタ
       */
      CameraComponent() = delete;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      CameraComponent(const CameraComponent& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      CameraComponent& operator = (const CameraComponent& other) = delete;

      /**
       *  @brief  コンポーネントの種別IDを取得する
       *  @return コンポーネントの種別ID
       */
      static constexpr std::uint32_t GetId()
      {
        // TODO:正しい値に修正
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
       *  @brief  注視点を設定する
       *  @param  look_at:注視点座標
       */
      void SetLookAt(DirectX::XMVECTOR look_at);

      /**
       *  @brief  注視点を取得する
       *  @return 注視点座標
       */
      DirectX::XMVECTOR GetLookAt();

    private:
      DirectX::XMVECTOR look_at_;  ///< 注視点座標
    };
  };
};
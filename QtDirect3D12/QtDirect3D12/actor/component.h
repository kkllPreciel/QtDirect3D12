/**
 *  @file     component.h
 *  @brief    コンポーネントに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/20
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include "actor/actor.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  コンポーネントクラス
     */
    class Component
    {
    public:
      /**
       *  @brief  コンポーネントの種別ID
       */
      enum ID : std::uint32_t
      {
        kIllegal = 0,
        kObject,      ///< オブジェクト
        kMove,        ///< 移動処理
      };

      /**
       *  @brief  デストラクタ
       */
      virtual ~Component();

      /**
       *  @brief  コンストラクタ
       */
      Component() = delete;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      Component(const Component& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      Component& operator = (const Component& other) = delete;

      /**
       *  @brief  初期化を行う
       *  @return 初期化成功フラグ
       */
      virtual bool Create() = 0;

      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

    protected:
      /**
       *  @brief  引数付きコンストラクタ
       *  @param  owner:コンポーネントを所持しているオーナー
       */
      Component(Actor* owner);

      Actor* owner_;  ///< オーナーアクター
    };
  };
};
/**
 *  @file     actor.h
 *  @brief    アクターに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/20
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include <map>
#include <DirectXMath.h>

namespace App
{
  namespace actor
  {
    class Component;

    /**
     *  @brief  アクタークラス
     */
    class Actor
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Actor();

      /**
       *  @brief  デストラクタ
       */
      virtual ~Actor();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      Actor(const Actor& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      Actor& operator = (const Actor& other) = delete;

      /**
       *  @brief  初期化を行う
       *  @return 初期化成功フラグ
       */
      bool Create();

      /**
       *  @brief  終了処理を行う
       */
      void Destroy();

      /**
       *  @brief  更新処理を行う
       *  @param  delta_time:1フレームの経過時間
       */
      void Update(std::uint64_t delta_time);

      /**
       *  @brief  ユニークIDを設定する
       *  @param  unique_id:ユニークID
       */
      void SetUniqueId(std::uint64_t unique_id);

      /**
       *  @brief  ユニークIDを取得する
       *  @return ユニークID
       */
      std::uint64_t GetUniqueId() const;

      /**
       *  @brief  座標を設定する
       *  @param  position:設定する座標
       */
      void SetPosition(DirectX::XMVECTOR position);

      /**
       *  @brief  座標を取得する
       *  @return 座標
       */
      DirectX::XMVECTOR GetPosition() const;

      /**
       *  @brief  回転を設定する
       *  @param  rotation:設定する回転
       */
      void SetRotation(DirectX::XMVECTOR rotation);

      /**
       *  @brief  回転を取得する
       *  @return 回転
       */
      DirectX::XMVECTOR GetRotation() const;

      /**
       *  @brief  コンポーネントを作成する
       */
      template<typename _Type> _Type* AddComponent();

      /**
       *  @brief  コンポーネントを取得する
       */
      template<typename _Type> _Type* GetComponent();

    private:
      std::uint64_t unique_id_;                                       ///< アクター間でのユニークなID
      std::map<std::uint32_t, Component*> components_;                ///< コンポーネント用コンテナ
      DirectX::XMVECTOR position_ = DirectX::XMVectorZero();          ///< 座標(ベクトル)
      DirectX::XMVECTOR rotation_ = DirectX::XMQuaternionIdentity();  ///< 回転(クォータニオン)
    };

    /**
     *  @brief  コンポーネントを作成する
     */
    template<typename _Type> _Type* Actor::AddComponent()
    {
      // デバッグ時専用のチェック処理
      // コンポーネントの二重登録は不許可
      assert(GetComponent<_Type>() == nullptr);

      _Type* component = new _Type(this);
      component->Create();
      components_.insert(std::pair<std::uint32_t, Component*>(_Type::GetId(), component));

      return component;
    }
    
    /**
     *  @brief  コンポーネントを取得する
     */
    template<typename _Type> _Type* Actor::GetComponent()
    {
      std::map<std::uint32_t, Component*>::iterator it = components_.find(_Type::GetId());
      if (it != components_.end())
      {
        it->second->Destroy();
        return static_cast<_Type*>(it->second);
      }

      return nullptr;
    }
  };
};
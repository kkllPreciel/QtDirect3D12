/**
 *  @file     camera_component.cc
 *  @brief    カメラ用コンポーネントに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/27
 *  @version  1.0
 */

 // include
#include "actor/camera_component.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  引数付きコンストラクタ
     *  @param  owner:コンポーネントを所持しているオーナー
     */
    CameraComponent::CameraComponent(Actor* owner) : Component(owner)
    {

    }

    /**
     *  @brief  デストラクタ
     */
    CameraComponent::~CameraComponent()
    {
      Destroy();
    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool CameraComponent::Create()
    {
      dirty_ = true;

      return true;
    }

    /**
     *  @brief  終了処理を行う
     */
    void CameraComponent::Destroy()
    {

    }

    /**
     *  @brief  注視点を設定する
     *  @param  look_at:注視点座標
     */
    void CameraComponent::SetLookAt(DirectX::XMVECTOR look_at)
    {
      look_at_ = look_at;
    }
    
    /**
     *  @brief  注視点を取得する
     *  @return 注視点座標
     */
    DirectX::XMVECTOR CameraComponent::GetLookAt()
    {
      return look_at_;
    }

    /**
     *  @brief  上方向を設定する
     *  @param  up:上方向
     */
    void CameraComponent::SetUpDirection(DirectX::XMVECTOR up)
    {
      up_ = up;
    }
    
    /**
     *  @brief  上方向を取得する
     *  @return 上方向ベクトル
     */
    DirectX::XMVECTOR CameraComponent::GetUpDirection()
    {
      return up_;
    }

    /**
     *  @brief  ビュー行列を取得する
     *  @return ビュー行列
     */
    const DirectX::XMMATRIX& CameraComponent::GetViewMatrix()
    {
      // ダーティ(更新)フラグが立っていない
      // キャッシュした行列で問題ない
      if (false == dirty_)
      {
        return matrix_;
      }

      dirty_ = false;

      // ビュー行列を計算する
      DirectX::XMVECTOR eye = owner_->GetPosition();
      DirectX::XMVECTOR at = GetLookAt();;
      DirectX::XMVECTOR up = GetUpDirection();
      matrix_ = DirectX::XMMatrixLookAtLH(eye, at, up);

      return matrix_;
    }

    /**
     *  @brief  ダーティフラグを設定する
     *  @param  dirty:ダーティフラグ
     */
    void CameraComponent::SetDirty(bool dirty)
    {
      dirty_ = dirty;
    }
  };
};
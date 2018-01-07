/**
 *  @file     model.h
 *  @brief    モデルに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/12/31
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include <vector>
#include <DirectXMath.h>

namespace App
{
  /**
   *  @brief  モデル構造インターフェイス
   */
  class IModel
  {
  public:
    /**
     *  @brief  デストラクタ
     */
    virtual ~IModel() {};

    /**
     *  @brief  頂点座標データを取得する
     *  @return 頂点座標データのリスト
     */
    virtual const std::vector<DirectX::XMFLOAT4A>& GetVertices() const = 0;

    /**
     *  @brief  頂点数を取得する
     *  @return 頂点数
     */
    virtual std::size_t GetVertexCount() const = 0;

    /**
     *  @brief  ポリゴン数を取得する
     *  @return ポリゴン数
     */
    virtual std::size_t GetPolygonCount() const = 0;

    /**
     *  @brief  頂点インデックスデータを取得する
     *  @return 頂点インデックスデータのリスト
     */
    virtual const std::vector<uint32_t>& GetIndices() const = 0;

    /**
     *  @brief  頂点インデックス数を取得する
     *  @return 頂点インデックス数
     */
    virtual std::size_t GetIndexCount() const = 0;

    /**
     *  @brief  データを開放する
     */
    virtual void Release() = 0;
  };
};
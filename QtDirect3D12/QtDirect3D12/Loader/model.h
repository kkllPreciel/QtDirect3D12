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
     *  @brief  頂点数を取得する
     *  @return 頂点数
     */
    virtual uint32_t GetVertexCount() = 0;

    /**
     *  @brief  ポリゴン数を取得する
     *  @return ポリゴン数
     */
    virtual uint32_t GetPolygonCount() = 0;

    /**
     *  @brief  頂点インデックス数を取得する
     *  @return 頂点インデックス数
     */
    virtual uint32_t GetIndexCount() = 0;

    /**
     *  @brief  データを開放する
     */
    virtual void Release() = 0;
  };
};
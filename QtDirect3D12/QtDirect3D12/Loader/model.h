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
#include <memory>
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
     *  @brief  頂点座標リストを取得する
     *  @return 頂点座標のリスト
     */
    virtual const std::vector<DirectX::XMFLOAT3>& GetControlPoints() const = 0;

    /**
     *  @brief  頂点座標数を取得する
     *  @return 頂点座標数
     */
    virtual uint32_t GetControlPointCount() const = 0;

    /**
     *  @brief  頂点法線ベクトルリストを取得する
     *  @return 頂点法線ベクトルのリスト
     */
    virtual const std::vector<DirectX::XMFLOAT3>& GetNormals() const = 0;

    /**
     *  @brief  頂点法線ベクトル数を取得する
     *  @return 頂点法線ベクトル数
     */
    virtual uint32_t GetNormalCount() const = 0;

    /**
     *  @brief  頂点テクスチャ座標リストを取得する
     *  @return 頂点テクスチャ座標のリスト
     */
    virtual const std::vector<DirectX::XMFLOAT2>& GetTextureCoords() const = 0;

    /**
     *  @brief  テクスチャ座標数を取得する
     *  @return テクスチャ座標数
     */
    virtual uint32_t GetTextureCoordCount() const = 0;

    /**
     *  @brief  ポリゴン数を取得する
     *  @return ポリゴン数
     */
    virtual uint32_t GetPolygonCount() const = 0;

    /**
     *  @brief  頂点インデックスデータを取得する
     *  @return 頂点インデックスデータのリスト
     */
    virtual const std::vector<uint32_t>& GetIndices() const = 0;

    /**
     *  @brief  頂点インデックス数を取得する
     *  @return 頂点インデックス数
     */
    virtual uint32_t GetIndexCount() const = 0;

    /**
     *  @brief  頂点法線ベクトルインデックスリストを取得する
     *  @return 頂点法線ベクトルインデックスのリスト
     */
    virtual const std::vector<uint32_t>& GetNormalIndices() const = 0;

    /**
     *  @brief  頂点法線ベクトルインデックス数を取得する
     *  @return 頂点法線ベクトルインデックス数
     */
    virtual uint32_t GetNormalIndexCount() const = 0;

    /**
     *  @brief  頂点テクスチャ座標インデックスリストを取得する
     *  @return 頂点テクスチャ座標インデックスのリスト
     */
    virtual const std::vector<uint32_t>& GetTextureCoordIndices() const = 0;

    /**
     *  @brief  頂点テクスチャ座標インデックス数を取得する
     *  @return 頂点テクスチャ座標インデックス数
     */
    virtual uint32_t GetTextureCoordIndexCount() const = 0;

    /**
     *  @brief  データを開放する
     */
    virtual void Release() = 0;

    /**
     *  @brief  OBJフォーマットファイルからモデルデータを作成する
     *  @param  file_path:読み込みを行うファイルのパス
     *  @return モデルインターフェイスへのポインタ
     */
    static std::unique_ptr<IModel> CreateFromObj(const std::string& file_path);
  };
};
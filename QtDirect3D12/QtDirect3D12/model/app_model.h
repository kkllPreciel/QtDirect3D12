/**
 *  @file     app_model.h
 *  @brief    アプリケーションで使用するモデルに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/02/15
 *  @version  1.0
 */

#pragma once

 // include
#include "model/model.h"

namespace App
{
  /**
   *  @brief  モデル構造インターフェイス
   */
  class IAppModel : public IModel
  {
  public:
    // アラインメントを1バイトに設定
#pragma pack(push, 1)
    struct Vertex
    {
      DirectX::XMFLOAT3 position; ///< 座標
      DirectX::XMFLOAT3 normal;   ///< 法線ベクトル
      DirectX::XMFLOAT2 texcoord; ///< テクスチャUV座標
    };
#pragma pack(pop)

  public:
    /**
     *  @brief  デストラクタ
     */
    virtual ~IAppModel() {};

    /**
     *  @brief  頂点リストを取得する
     *  @return 頂点のリスト
     */
    virtual const std::vector<Vertex>& GetVertices() const = 0;

    /**
     *  @brief  頂点インデックスリストを取得する
     *  @return 頂点インデックスのリスト
     */
    virtual const std::vector<uint32_t>& GetIndices() const = 0;

    /**
     *  @brief  OBJフォーマットファイルからモデルデータを作成する
     *  @param  file_path:読み込みを行うファイルのパス
     *  @return モデルインターフェイスへのポインタ
     */
    static std::unique_ptr<IAppModel> LoadFromObj(const std::string& file_path);
  };
};
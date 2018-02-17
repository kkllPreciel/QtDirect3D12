/**
 *  @file     app_model.h
 *  @brief    アプリケーションで使用するモデルに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/02/15
 *  @version  1.0
 */

#pragma once

 // include
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>
#include <Sein/Direct3D12/direct3d12_device.h>
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
     *  @brief  頂点バッファを取得する
     *  @return 頂点バッファ
     */
    virtual const Sein::Direct3D12::VertexBuffer& GetVertexBuffer() const = 0;

    /**
     *  @brief  インデックスバッファを取得する
     *  @return インデックスバッファ
     */
    virtual const Sein::Direct3D12::IndexBuffer& GetIndexBuffer() const = 0;

    /**
     *  @brief  OBJフォーマットファイルからモデルデータを作成する
     *  @param  device:デバイス
     *  @param  file_path:読み込みを行うファイルのパス
     *  @return モデルインターフェイスへのポインタ
     */
    static std::unique_ptr<IAppModel> LoadFromObj(const Sein::Direct3D12::Device& device, const std::string& file_path);

    /**
     *  @brief  PMXフォーマットファイルからモデルデータを作成する
     *  @param  device:デバイス
     *  @param  file_path:読み込みを行うファイルのパス
     *  @return モデルインターフェイスへのポインタ
     */
    static std::unique_ptr<IAppModel> LoadFromPmx(const Sein::Direct3D12::Device& device, const std::string& file_path);
  };
};
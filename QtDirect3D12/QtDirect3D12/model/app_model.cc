/**
 *  @file     app_model.cc
 *  @brief    アプリケーションで使用するモデルに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/02/15
 *  @version  1.0
 */

 // include
#include "model/app_model.h"
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>

namespace App
{
  namespace
  {
    /**
     *  @brief  モデル用クラス
     */
    class AppModel final : public IAppModel
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      AppModel() : vertices_(0), indices_(0), model_(nullptr), vertex_buffer_(nullptr), index_buffer_(nullptr)
      {
    
      }
    
      /**
       *  @brief  デストラクタ
       */
      ~AppModel() override
      {
        Release();
      }
    
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      AppModel(const AppModel& other) = delete;
    
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      AppModel& operator = (const AppModel& other) = delete;
    
      /**
       *  @brief  ムーブコンストラクタ
       *  @param  other:ムーブ元のインスタンス
       */
      AppModel(AppModel&& other) = delete;
    
      /**
       *  @brief  ムーブ代入演算子オペレータ
       *  @param  other:ムーブ代入元のインスタンス
       *  @return ムーブ代入後のインスタンス
       */
      AppModel& operator = (AppModel&& other) = delete;

      /**
       *  @brief  頂点リストを取得する
       *  @return 頂点のリスト
       */
      const std::vector<Vertex>& GetVertices() const override
      {
        return vertices_;
      }
      
      /**
       *  @brief  頂点インデックスリストを取得する
       *  @return 頂点インデックスのリスト
       */
      const std::vector<uint32_t>& GetIndices() const override
      {
        return indices_;
      }

      /**
       *  @brief  頂点バッファを取得する
       *  @return 頂点バッファ
       */
      const Sein::Direct3D12::VertexBuffer& GetVertexBuffer() const override
      {
        return *vertex_buffer_;
      }
  
      /**
       *  @brief  インデックスバッファを取得する
       *  @return インデックスバッファ
       */
      const Sein::Direct3D12::IndexBuffer& GetIndexBuffer() const override
      {
        return *index_buffer_;
      }
      
      /**
       *  @brief  ポリゴン数を取得する
       *  @return ポリゴン数
       */
      uint32_t GetPolygonCount() const override
      {
        return model_->GetPolygonCount();
      }
    
      /**
       *  @brief  頂点座標リストを取得する
       *  @return 頂点座標のリスト
       */
      const std::vector<DirectX::XMFLOAT3>& GetControlPoints() const override
      {
        return model_->GetControlPoints();
      }
    
      /**
       *  @brief  頂点座標数を取得する
       *  @return 頂点座標数
       */
      uint32_t GetControlPointCount() const override
      {
        return  model_->GetControlPointCount();
      }
    
      /**
       *  @brief  頂点法線ベクトルリストを取得する
       *  @return 頂点法線ベクトルのリスト
       */
      const std::vector<DirectX::XMFLOAT3>& GetNormals() const override
      {
        return model_->GetNormals();
      }
    
      /**
       *  @brief  頂点法線ベクトル数を取得する
       *  @return 頂点法線ベクトル数
       */
      uint32_t GetNormalCount() const override
      {
        return model_->GetNormalCount();
      }
    
      /**
       *  @brief  頂点テクスチャ座標リストを取得する
       *  @return 頂点テクスチャ座標のリスト
       */
      const std::vector<DirectX::XMFLOAT2>& GetTextureCoords() const override
      {
        return model_->GetTextureCoords();
      }
    
      /**
       *  @brief  テクスチャ座標数を取得する
       *  @return テクスチャ座標数
       */
      uint32_t GetTextureCoordCount() const override
      {
        return model_->GetTextureCoordCount();
      }

      /**
       *  @brief  頂点座標インデックスデータを取得する
       *  @return 頂点座標インデックスデータのリスト
       */
      const std::vector<uint32_t>& GetControlPointIndices() const override
      {
        return model_->GetControlPointIndices();
      }
      
      /**
       *  @brief  頂点座標インデックス数を取得する
       *  @return 頂点座標インデックス数
       */
      uint32_t GetControlPointIndexCount() const override
      {
        return model_->GetControlPointIndexCount();
      }
    
      /**
       *  @brief  頂点法線ベクトルインデックスリストを取得する
       *  @return 頂点法線ベクトルインデックスのリスト
       */
      const std::vector<uint32_t>& GetNormalIndices() const override
      {
        return model_->GetNormalIndices();
      }
    
      /**
       *  @brief  頂点法線ベクトルインデックス数を取得する
       *  @return 頂点法線ベクトルインデックス数
       */
      uint32_t GetNormalIndexCount() const override
      {
        return model_->GetNormalIndexCount();
      }
    
      /**
       *  @brief  頂点テクスチャ座標インデックスリストを取得する
       *  @return 頂点テクスチャ座標インデックスのリスト
       */
      const std::vector<uint32_t>& GetTextureCoordIndices() const override
      {
        return model_->GetTextureCoordIndices();
      }
    
      /**
       *  @brief  頂点テクスチャ座標インデックス数を取得する
       *  @return 頂点テクスチャ座標インデックス数
       */
      uint32_t GetTextureCoordIndexCount() const override
      {
        return model_->GetTextureCoordIndexCount();
      }
    
      /**
       *  @brief  データを開放する
       */
      void Release() override
      {
        vertices_.clear();
        indices_.clear();

        vertex_buffer_.reset();
        index_buffer_.reset();

        model_->Release();
        model_.reset();
      }

      /**
       *  @brief  頂点を追加する
       *  @param  vertex:追加する頂点
       */
      void AddVertex(const Vertex& vertex)
      {
        vertices_.emplace_back(vertex);
      }

      /**
       *  @brief  頂点インデックスを追加する
       *  @param  indices:追加する頂点インデックスのポリゴン単位でのリスト
       */
      void AddIndex(const std::vector<uint32_t>& polygon_indices)
      {
        for (auto index : polygon_indices)
        {
          indices_.emplace_back(index);
        }
      }

      /**
       *  @brief  モデルを設定する
       *  @param  model:モデル
       */
      void SetModel(App::IModel* model)
      {
        model_.reset(model);
      }

      /**
       *  @brief  頂点バッファを設定する
       *  @param  vertex_buffer:頂点バッファ
       */
      void SetVertexBuffer(Sein::Direct3D12::VertexBuffer* vertex_buffer)
      {
        vertex_buffer_.reset(vertex_buffer);
      }

      /**
       *  @brief  インデックスバッファを設定する
       *  @param  index_buffer:インデックスバッファ
       */
      void SetIndexBuffer(Sein::Direct3D12::IndexBuffer* index_buffer)
      {
        index_buffer_.reset(index_buffer);
      }
    
    private:
      std::vector<Vertex> vertices_;                                  ///< 頂点リスト
      std::vector<uint32_t> indices_;                                 ///< インデックスリスト
      std::unique_ptr<App::IModel> model_;                            ///< モデル
      std::unique_ptr<Sein::Direct3D12::VertexBuffer> vertex_buffer_; ///< 頂点バッファ
      std::unique_ptr<Sein::Direct3D12::IndexBuffer> index_buffer_;   ///< インデックスバッファ
    };
  };

  /**
   *  @brief  OBJフォーマットファイルからモデルデータを作成する
   *  @param  device:デバイス
   *  @param  file_path:読み込みを行うファイルのパス
   *  @return モデルインターフェイスへのポインタ
   */
  std::unique_ptr<IAppModel> IAppModel::LoadFromObj(const Sein::Direct3D12::Device& device, const std::string& file_path)
  {
    std::unique_ptr<AppModel> app_model = nullptr;

    auto model = IModel::CreateFromObj(file_path);
    if (!model)
    {
      return app_model;
    }

    app_model = std::make_unique<AppModel>();
    app_model->SetModel(model.release());

    // アプリケーション用の頂点データを生成する
    std::vector<Vertex> vertices;
    std::unordered_map<std::string, uint32_t> map;
    std::vector<uint32_t> indices;

    const auto points = app_model->GetControlPoints();
    const auto normals = app_model->GetNormals();
    const auto tex_coords = app_model->GetTextureCoords();
    const auto point_indices = app_model->GetControlPointIndices();
    const auto normal_indices = app_model->GetNormalIndices();
    const auto tex_coords_indices = app_model->GetTextureCoordIndices();

    // 頂点データリストと、頂点データインデックスとハッシュを持つリストを作成する
    for (const auto point : point_indices | boost::adaptors::indexed())
    {
      const auto index = point.index();

      // ハッシュを作成
      std::string hash = std::to_string(point.value());

      if (tex_coords_indices.empty() == false)
      {
        hash += '-' + std::to_string(tex_coords_indices[index]);
      }

      if (normal_indices.empty() == false)
      {
        hash += '-' + std::to_string(normal_indices[index]);
      }

      // ハッシュが存在しない場合は頂点データを追加する
      if (map.count(hash) == 0)
      {
        // 頂点データ
        Vertex vertex = {};
        vertex.position = points.at(point.value());

        if (normal_indices.empty() == false)
        {
          vertex.normal = normals.at(normal_indices.at(index));
        }

        if (tex_coords_indices.empty() == false)
        {
          vertex.texcoord = tex_coords.at(tex_coords_indices.at(index));
        }

        vertices.emplace_back(vertex);

        // ハッシュリストを更新
        map.insert({ hash, static_cast<uint32_t>(vertices.size() - 1) });
      }

      // インデックスリストにインデックスを追加する
      indices.emplace_back(map.at(hash));
    }

    auto vertex_size = sizeof(App::IAppModel::Vertex);
    auto vertex_buffer = std::make_unique<Sein::Direct3D12::VertexBuffer>();
    auto index_buffer = std::make_unique<Sein::Direct3D12::IndexBuffer>();
    vertex_buffer->Create(&(device.GetDevice()), vertex_size * vertices.size(), vertex_size, &(vertices[0]));
    index_buffer->Create(&(device.GetDevice()), sizeof(uint32_t) * indices.size(), &(indices[0]), DXGI_FORMAT_R32_UINT);

    for (decltype(auto) vertex : vertices)
    {
      app_model->AddVertex(vertex);
    }

    app_model->AddIndex(indices);

    app_model->SetVertexBuffer(vertex_buffer.release());
    app_model->SetIndexBuffer(index_buffer.release());

    return app_model;
  }

  /**
   *  @brief  PMXフォーマットファイルからモデルデータを作成する
   *  @param  device:デバイス
   *  @param  file_path:読み込みを行うファイルのパス
   *  @return モデルインターフェイスへのポインタ
   */
  std::unique_ptr<IAppModel> IAppModel::LoadFromPmx(const Sein::Direct3D12::Device& device, const std::string& file_path)
  {
    std::unique_ptr<AppModel> app_model = nullptr;

    auto model = IModel::CreateFromPmx(file_path);
    if (!model)
    {
      return app_model;
    }

    app_model = std::make_unique<AppModel>();
    app_model->SetModel(model.release());

    // アプリケーション用の頂点データを生成する
    std::vector<Vertex> vertices(app_model->GetControlPointCount());
    std::vector<uint32_t> indices(app_model->GetControlPointIndexCount());

    // 頂点生成
    const auto points = app_model->GetControlPoints();
    const auto normals = app_model->GetNormals();
    const auto tex_coords = app_model->GetTextureCoords();
    for (auto& vertex : vertices | boost::adaptors::indexed())
    {
      const auto index = vertex.index();

      vertex.value().position = points[index];
      vertex.value().normal = normals[index];
      vertex.value().texcoord = tex_coords[index];
    }

    // インデックス生成
    const auto point_indices = app_model->GetControlPointIndices();
    for (auto& index : indices | boost::adaptors::indexed())
    {
      index.value() = point_indices[index.index()];
    }

    auto vertex_size = sizeof(App::IAppModel::Vertex);
    auto vertex_buffer = std::make_unique<Sein::Direct3D12::VertexBuffer>();
    auto index_buffer = std::make_unique<Sein::Direct3D12::IndexBuffer>();
    vertex_buffer->Create(&(device.GetDevice()), vertex_size * vertices.size(), vertex_size, &(vertices[0]));
    index_buffer->Create(&(device.GetDevice()), sizeof(uint32_t) * indices.size(), &(indices[0]), DXGI_FORMAT_R32_UINT);

    for (decltype(auto) vertex : vertices)
    {
      app_model->AddVertex(vertex);
    }

    app_model->AddIndex(indices);

    app_model->SetVertexBuffer(vertex_buffer.release());
    app_model->SetIndexBuffer(index_buffer.release());

    return app_model;
  }
};
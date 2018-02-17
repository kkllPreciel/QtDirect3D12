/**
 *  @file     obj_model_loader.cc
 *  @brief    OBJファイル読み込みに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/01
 *  @version  1.0
 */

 // include
#include "model/model.h"
#include <fstream>
#include <sstream>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>

namespace App
{
  namespace
  {
    enum ObjKeywords {
      kIllegal = 0,
      kComment,
      kMaterialFileName,
      kGroup,
      kUseMaterial,
      kControlPoint,
      kVertexTexture,
      kVertexNormal,
      kPolygon,
    };

    ObjKeywords GetID(std::string keyword)
    {
      std::map<std::string, ObjKeywords> map =
      {
        { "#", ObjKeywords::kComment },
        { "mtllib", ObjKeywords::kMaterialFileName },
        { "g", ObjKeywords::kGroup },
        { "usemtl", ObjKeywords::kUseMaterial },
        { "v", ObjKeywords::kControlPoint },
        { "vt", ObjKeywords::kVertexTexture },
        { "vn", ObjKeywords::kVertexNormal },
        { "f", ObjKeywords::kPolygon },
      };

      if (map.count(keyword) == 0)
      {
        return ObjKeywords::kIllegal;
      }

      return map.at(keyword);
    }

    /**
     *  @brief  OBJファイルモデル用クラス
     */
    class Model final : public IModel
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Model() : polygon_count_(0), points_(0), normals_(0), texture_coords_(0),
        point_indices_(0), normal_indices_(0), texture_coord_indices_(0)
      {
    
      }
    
      /**
       *  @brief  デストラクタ
       */
      ~Model() override
      {
        Release();
      }
    
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      Model(const Model& other) = delete;
    
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      Model& operator = (const Model& other) = delete;
    
      /**
       *  @brief  ムーブコンストラクタ
       *  @param  other:ムーブ元のインスタンス
       */
      Model(Model&& other) = delete;
    
      /**
       *  @brief  ムーブ代入演算子オペレータ
       *  @param  other:ムーブ代入元のインスタンス
       *  @return ムーブ代入後のインスタンス
       */
      Model& operator = (Model&& other) = delete;
      
      /**
       *  @brief  ポリゴン数を取得する
       *  @return ポリゴン数
       */
      uint32_t GetPolygonCount() const override
      {
        return polygon_count_;
      }
    
      /**
       *  @brief  頂点座標リストを取得する
       *  @return 頂点座標のリスト
       */
      const std::vector<DirectX::XMFLOAT3>& GetControlPoints() const override
      {
        return points_;
      }
    
      /**
       *  @brief  頂点座標数を取得する
       *  @return 頂点座標数
       */
      uint32_t GetControlPointCount() const override
      {
        return static_cast<uint32_t>(points_.size());
      }
    
      /**
       *  @brief  頂点法線ベクトルリストを取得する
       *  @return 頂点法線ベクトルのリスト
       */
      const std::vector<DirectX::XMFLOAT3>& GetNormals() const override
      {
        return normals_;
      }
    
      /**
       *  @brief  頂点法線ベクトル数を取得する
       *  @return 頂点法線ベクトル数
       */
      uint32_t GetNormalCount() const override
      {
        return static_cast<uint32_t>(normals_.size());
      }
    
      /**
       *  @brief  頂点テクスチャ座標リストを取得する
       *  @return 頂点テクスチャ座標のリスト
       */
      const std::vector<DirectX::XMFLOAT2>& GetTextureCoords() const override
      {
        return texture_coords_;
      }
    
      /**
       *  @brief  テクスチャ座標数を取得する
       *  @return テクスチャ座標数
       */
      uint32_t GetTextureCoordCount() const override
      {
        return static_cast<uint32_t>(texture_coords_.size());
      }

      /**
       *  @brief  頂点座標インデックスデータを取得する
       *  @return 頂点座標インデックスデータのリスト
       */
      const std::vector<uint32_t>& GetControlPointIndices() const override
      {
        return point_indices_;
      }
      
      /**
       *  @brief  頂点座標インデックス数を取得する
       *  @return 頂点座標インデックス数
       */
      uint32_t GetControlPointIndexCount() const override
      {
        return static_cast<uint32_t>(point_indices_.size());
      }
    
      /**
       *  @brief  頂点法線ベクトルインデックスリストを取得する
       *  @return 頂点法線ベクトルインデックスのリスト
       */
      const std::vector<uint32_t>& GetNormalIndices() const override
      {
        return normal_indices_;
      }
    
      /**
       *  @brief  頂点法線ベクトルインデックス数を取得する
       *  @return 頂点法線ベクトルインデックス数
       */
      uint32_t GetNormalIndexCount() const override
      {
        return static_cast<uint32_t>(normal_indices_.size());
      }
    
      /**
       *  @brief  頂点テクスチャ座標インデックスリストを取得する
       *  @return 頂点テクスチャ座標インデックスのリスト
       */
      const std::vector<uint32_t>& GetTextureCoordIndices() const override
      {
        return texture_coord_indices_;
      }
    
      /**
       *  @brief  頂点テクスチャ座標インデックス数を取得する
       *  @return 頂点テクスチャ座標インデックス数
       */
      uint32_t GetTextureCoordIndexCount() const override
      {
        return static_cast<uint32_t>(texture_coord_indices_.size());
      }
    
      /**
       *  @brief  データを開放する
       */
      void Release() override
      {
        polygon_count_ = 0;        
        points_.clear();
        normals_.clear();
        texture_coords_.clear();
        point_indices_.clear();
        normal_indices_.clear();
        texture_coord_indices_.clear();
      }

      /**
       *  @brief  ポリゴンを追加する
       */
      void AddPolygon()
      {
        ++polygon_count_;
      }
    
      /**
       *  @brief  座標を追加する
       *  @param  point:追加する座標
       */
      void AddControlPoint(const DirectX::XMFLOAT3& point)
      {
        points_.emplace_back(point);
      }
    
      /**
       *  @brief  法線ベクトルを追加する
       *  @param  normal:追加する法線ベクトル
       */
      void AddNormal(const DirectX::XMFLOAT3& normal)
      {
        normals_.emplace_back(normal);
      }
    
      /**
       *  @brief  テクスチャ座標を追加する
       *  @param  texture_coord:追加するテクスチャ座標
       */
      void AddTextureCoord(const DirectX::XMFLOAT2& texture_coord)
      {
        texture_coords_.emplace_back(texture_coord);
      }
    
      /**
       *  @brief  頂点座標インデックスを追加する
       *  @param  polygon_indices:追加する頂点座標インデックスのポリゴン単位でのリスト
       */
      void AddPointIndex(const std::vector<uint32_t>& polygon_indices)
      {
        for (auto index : polygon_indices)
        {
          point_indices_.emplace_back(index);
        }
      }
    
      /**
       *  @brief  頂点法線ベクトルインデックスを追加する
       *  @param  polygon_indices:頂点法線ベクトルインデックスのポリゴン単位でのリスト
       */
      void AddNormalIndex(const std::vector<uint32_t>& polygon_indices)
      {
        for (auto index : polygon_indices)
        {
          normal_indices_.emplace_back(index);
        }
      }
    
      /**
       *  @brief  頂点テクスチャ座標インデックスを追加する
       *  @param  polygon_indices:追加する頂点テクスチャ座標インデックスのポリゴン単位でのリスト
       */
      void AddTextureCoordIndex(const std::vector<uint32_t>& polygon_indices)
      {
        for (auto index : polygon_indices)
        {
          texture_coord_indices_.emplace_back(index);
        }
      }
    
    private:
      uint32_t polygon_count_;                        ///< ポリゴン数
      std::vector<DirectX::XMFLOAT3> points_;         ///< 座標リスト
      std::vector<DirectX::XMFLOAT3> normals_;        ///< 法線ベクトルリスト
      std::vector<DirectX::XMFLOAT2> texture_coords_; ///< テクスチャ座標リスト
      std::vector<uint32_t> point_indices_;           ///< 頂点座標インデックスリスト
      std::vector<uint32_t> normal_indices_;          ///< 頂点法線ベクトルインデックスリスト
      std::vector<uint32_t> texture_coord_indices_;   ///< 頂点テクスチャ座標インデックスリスト
    };
  };

  /**
   *  @brief  OBJフォーマットファイルからモデルデータを作成する
   *  @param  file_path:読み込みを行うファイルのパス
   *  @return モデルインターフェイスへのポインタ
   */
  std::unique_ptr<IModel> IModel::CreateFromObj(const std::string& file_path)
  {
    std::unique_ptr<Model> model = nullptr;

    // TODO:拡張子チェック

    std::ifstream stream;
    stream.open(file_path, std::ifstream::in | std::ifstream::binary);
    if (false == stream.operator bool())
    {
      return model;
    }
    
    // 読み込み実行
    std::istreambuf_iterator<char> it(stream);
    std::istreambuf_iterator<char> last;
    std::string data(it, last);
    stream.close();

    // C4996警告を無効にする
#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(push)
#pragma warning(disable:4996)
#endif
    // 行毎に分割する
    std::vector<std::string> list;
    boost::split(list, data, boost::is_any_of("\r\n"));
#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(pop)
#endif

    model = std::make_unique<Model>();

    // モデルデータ読み込み(行単位で処理を行う)
    for (auto line : list)
    {
      std::vector<std::string> parts;
      boost::split(parts, line, boost::is_space());

      // 分割出来なかった(違法なデータ)
      if (parts.size() <= 1)
      {
        continue;
      }

      // キーワードを取得し対応する処理を行う
      switch (GetID(parts.at(0)))
      {
        // コメント
      case ObjKeywords::kComment:
        continue;
        break;
        // マテリアルファイル名
      case ObjKeywords::kMaterialFileName:
        continue;
        break;
        // グループ
      case ObjKeywords::kGroup:
        continue;
        break;
        // 現在のグループデータが使用するマテリアル名
      case ObjKeywords::kUseMaterial:
        break;
        // 頂点座標
      case ObjKeywords::kControlPoint:
        // TODO:要素数が4でない場合は違法データとして戻す
        model->AddControlPoint(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
        break;
        // テクスチャ座標
      case ObjKeywords::kVertexTexture:
        // TODO:要素数が3でない場合は違法データとして戻す
        model->AddTextureCoord(DirectX::XMFLOAT2(std::stof(parts[1]), std::stof(parts[2])));
        break;
        // 頂点法線ベクトル
      case ObjKeywords::kVertexNormal:
        // TODO:要素数が4でない場合は違法データとして戻す
        model->AddNormal(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
        break;
        // ポリゴンデータ
      case ObjKeywords::kPolygon:
      {
        std::vector<uint32_t> point_indices;
        std::vector<uint32_t> normal_indices;
        std::vector<uint32_t> texture_coord_indices;
        for (decltype(parts.size()) i = 1; i < parts.size(); ++i)
        {
          std::vector<std::string> indices;
          boost::split(indices, parts[i], boost::is_any_of("/"));
          for (auto index : indices | boost::adaptors::indexed())
          {
            if (false == index.value().empty())
            {
              // 座標・テクスチャ座標・法線ベクトルは0番目からなので、-1を行う
              const auto value = std::stoi(index.value()) - 1;
              switch (index.index())
              {
              case 0:

                point_indices.emplace_back(value);
                break;
              case 1:
                texture_coord_indices.emplace_back(value);
                break;
              case 2:
                normal_indices.emplace_back(value);
                break;
              }
            }
          }
        }
        model->AddPointIndex(point_indices);
        model->AddTextureCoordIndex(texture_coord_indices);
        model->AddNormalIndex(normal_indices);
        model->AddPolygon();
      }
      break;
      // 不明キーワード
      default:
        continue;
        break;
      }
    }

    return model;
  }
};
/**
 *  @file     obj_loader.cc
 *  @brief    OBJファイル読み込みに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/01
 *  @version  1.0
 */

 // include
#include "obj_loader.h"
#include <fstream>
#include <sstream>
#include <map>
#include <boost/1_66_0/boost/algorithm/string.hpp>

namespace App
{
  namespace Loader
  {
    namespace Obj
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
          std::map<std::string, ObjKeywords> map = {
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
      };

      /**
       *  @brief  OBJファイルモデル用クラス
       */
      class Model final : public IModel
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Model() : points_(0), polygon_count_(0), indices_(0), normals_(0), texture_coords_(0)
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
        std::size_t GetControlPointCount() const override
        {
          return points_.size();
        }

        /**
         *  @brief  ポリゴン数を取得する
         *  @return ポリゴン数
         */
        std::size_t GetPolygonCount() const override
        {
          return polygon_count_;
        }

        /**
         *  @brief  頂点インデックスデータを取得する
         *  @return 頂点インデックスデータのリスト
         */
        const std::vector<uint32_t>& GetIndices() const override
        {
          return indices_;
        }

        /**
         *  @brief  頂点インデックス数を取得する
         *  @return 頂点インデックス数
         */
        std::size_t GetIndexCount() const override
        {
          return indices_.size();
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
        std::size_t GetNormalCount() const override
        {
          return normals_.size();
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
        std::size_t GetTextureCoordCount() const override
        {
          return texture_coords_.size();
        }

        /**
         *  @brief  データを開放する
         */
        void Release() override
        {
          points_.clear();
          polygon_count_ = 0;
          indices_.clear();
          normals_.clear();
          texture_coords_.clear();
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
         *  @brief  ポリゴンを追加する
         */
        void AddPolygon()
        {
          ++polygon_count_;
        }

        /**
         *  @brief  頂点インデックスを追加する
         *  @param  polygon_indices:追加する頂点インデックスのポリゴン単位でのリスト
         */
        void AddIndex(const std::vector<uint32_t>& polygon_indices)
        {
          for (auto index : polygon_indices)
          {
            indices_.emplace_back(index);
          }
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

      private:
        std::vector<DirectX::XMFLOAT3> points_;         ///< 座標リスト
        std::size_t polygon_count_;                     ///< ポリゴン数
        std::vector<uint32_t> indices_;                 ///< 頂点インデックスリスト
        std::vector<DirectX::XMFLOAT3> normals_;        ///< 法線ベクトルリスト
        std::vector<DirectX::XMFLOAT2> texture_coords_; ///< テクスチャ座標リスト
      };

      /**
       *  @brief  OBJファイル読み込み用クラス
       */
      class Loader final : public ILoader
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Loader()
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~Loader() override
        {

        }

        /**
         *  @brief  コピーコンストラクタ
         *  @param  other:コピー元のインスタンス
         */
        Loader(const Loader& other) = delete;

        /**
         *  @brief  コピー代入演算子オペレータ
         *  @param  other:コピー代入元のインスタンス
         *  @return コピー代入後のインスタンス
         */
        Loader& operator = (const Loader& other) = delete;

        /**
         *  @brief  ムーブコンストラクタ
         *  @param  other:ムーブ元のインスタンス
         */
        Loader(Loader&& other) = delete;

        /**
         *  @brief  ムーブ代入演算子オペレータ
         *  @param  other:ムーブ代入元のインスタンス
         *  @return ムーブ代入後のインスタンス
         */
        Loader& operator = (Loader&& other) = delete;

        /**
         *  @brief  読み込みを行う
         *  @param  file_path:読み込みを行うファイルのパス
         *  @param  status_code:読み込み結果のステータスコード
         *  @return モデルインターフェイスへのポインタ
         */
        std::unique_ptr<IModel> Load(const std::string& file_path, StatusCode* const status_code) override
        {
          std::unique_ptr<Model> model = nullptr;
          if (nullptr != status_code)
          {
            *status_code = StatusCode::UNSUPPORTED;
          }

          // 拡張子チェック


          if (nullptr != status_code)
          {
            *status_code = StatusCode::FILE_NOT_FOUND;
          }

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
                std::vector<uint32_t> indices;
                for (decltype(parts.size()) i = 1; i < parts.size(); ++i)
                {
                  // 頂点は0番目からなので、(頂点インデックス - 1)を行う
                  indices.emplace_back(std::stoi(parts[i].substr(0, parts[i].find('/'))) - 1);
                }
                model->AddIndex(indices);
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

      /**
       *  @brief  OBJファイル読み込み用インスタンスを生成する
       *  @return 読み込み用インターフェイス
       */
      std::unique_ptr<ILoader> CreateLoader()
      {
        return std::make_unique<Loader>();
      }
    };
  };
};
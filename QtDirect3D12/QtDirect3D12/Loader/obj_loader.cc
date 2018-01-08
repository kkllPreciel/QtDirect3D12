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
          kVertex,
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
            { "v", ObjKeywords::kVertex },
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
        Model() : vertices_(0), polygon_count_(0), indices_(0)
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
         *  @brief  頂点座標データを取得する
         *  @return 頂点座標データのリスト
         */
        const std::vector<DirectX::XMFLOAT4A>& GetVertices() const override
        {
          return vertices_;
        }

        /**
         *  @brief  頂点数を取得する
         *  @return 頂点数
         */
        std::size_t GetVertexCount() const override
        {
          return vertices_.size();
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
         *  @brief  データを開放する
         */
        void Release() override
        {
          vertices_.clear();
          polygon_count_ = 0;
          indices_.clear();
        }

        /**
         *  @brief  頂点座標を追加する
         *  @param  vertex:追加する頂点座標
         */
        void AddVertex(const DirectX::XMFLOAT4A& vertex)
        {
          vertices_.emplace_back(vertex);
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

      private:
        std::vector<DirectX::XMFLOAT4A> vertices_;  ///< 頂点座標リスト
        std::size_t polygon_count_;                 ///< ポリゴン数
        std::vector<uint32_t> indices_;             ///< 頂点インデックスリスト
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
            case ObjKeywords::kVertex:
              // TODO:要素数が4でない場合は違法データとして戻す
              model->AddVertex(DirectX::XMFLOAT4A(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]), 0.0f));
              break;
              // テクスチャ座標
            case ObjKeywords::kVertexTexture:
              break;
              // 頂点法線ベクトル
            case ObjKeywords::kVertexNormal:
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
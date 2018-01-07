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

          // モデルデータ読み込み(行単位で処理を行う)
          std::stringstream ss(data);
          std::string datum;
          while (std::getline(ss, datum, '\n'))
          {
            // スペースまでの文字数を取得
            auto position = datum.find(' ');

            // スペースが存在しない(対応しない)
            if (std::string::npos == position)
            {
              continue;
            }

            // キーワードを取得し対応する処理を行う
            switch (GetID(datum.substr(0, position)))
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
              break;
              // テクスチャ座標
            case ObjKeywords::kVertexTexture:
              break;
              // 頂点法線ベクトル
            case ObjKeywords::kVertexNormal:
              break;
              // ポリゴンデータ
            case ObjKeywords::kPolygon:
              break;
              // 不明キーワード
            default:
              continue;
              break;
            }
          }

          // ダミーデータを作成し返す
          model = std::make_unique<Model>();
          model->AddVertex(DirectX::XMFLOAT4A(1.0f, -1.0f, -1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(1.0f, -1.0f, 1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(-1.0f, -1.0f, 1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(-1.0f, -1.0f, -1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(1.0f, 1.0f, -1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(1.0f, 1.0f, 1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(-1.0f, 1.0f, 1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(-1.0f, 1.0f, -1.0f, 0.0f));

          for (auto i = 0; i < 12; ++i)
            model->AddPolygon();

          model->AddIndex(std::vector<uint32_t> {
            1, 2, 3,
            1, 3, 4,
            5, 8, 7,
            5, 7, 6,
            1, 5, 6,
            1, 6, 2,
            2, 6, 7,
            2, 7, 3,
            3, 7, 8,
            3, 8, 4,
            5, 1, 4,
            5, 4, 8,
          });

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
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
      /**
       *  @brief  OBJファイルモデル用クラス
       */
      class Model final : public IModel
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Model() : vertex_count_(0), polygon_count_(0), index_count_(0)
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~Model() override
        {

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
         *  @brief  頂点数を取得する
         *  @return 頂点数
         */
        uint32_t GetVertexCount() override
        {
          return vertex_count_;
        }

        /**
         *  @brief  頂点数を設定する
         *  @param  vertex_count:頂点数
         */
        void SetVertexCount(uint32_t vertex_count)
        {
          vertex_count_ = vertex_count;
        }

        /**
         *  @brief  ポリゴン数を取得する
         *  @return ポリゴン数
         */
        uint32_t GetPolygonCount() override
        {
          return polygon_count_;
        }

        /**
         *  @brief  ポリゴン数を設定する
         *  @param  polygon_count:ポリゴン数
         */
        void SetPolygonCount(uint32_t polygon_count)
        {
          polygon_count_ = polygon_count;
        }

        /**
         *  @brief  頂点インデックス数を取得する
         *  @return 頂点インデックス数
         */
        uint32_t GetIndexCount() override
        {
          return index_count_;
        }

        /**
         *  @brief  頂点インデックス数を設定する
         *  @param  index_count:頂点インデックス数
         */
        void SetIndexCount(uint32_t index_count)
        {
          index_count_ = index_count;
        }

        /**
         *  @brief  データを開放する
         */
        void Release() override
        {

        }

      private:
        uint32_t vertex_count_;
        uint32_t polygon_count_;
        uint32_t index_count_;
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

            // キーワードのマップ(TODO:定数化)
            std::map<std::string, uint8_t> map = {
              { "#", 1 },
              { "mtllib", 2 },
              { "g", 3 },
              { "usemtl", 4 },
              { "v", 5 },
              { "vt", 6 },
              { "vn", 7 },
              { "f", 8 },
            };

            // キーワードを取得し対応する処理を行う
            switch (map.at(datum.substr(0, position)))
            {
            case 1: // コメント
              continue;
              break;
            case 2: // マテリアルファイル名
              continue;
              break;              
            case 3: // グループ
              continue;
              break;
            case 4: // 現在のグループデータが使用するマテリアル名
              break;
            case 5: // 頂点座標
              break;
            case 6: // テクスチャ座標
              break;
            case 7: // 頂点法線ベクトル
              break;
            case 8: // ポリゴンデータ
              break;
            default:
              break;
            }
          }

          model = std::make_unique<Model>();
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
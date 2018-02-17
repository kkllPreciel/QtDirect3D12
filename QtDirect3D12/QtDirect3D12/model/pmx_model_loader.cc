/**
 *  @file     pmx_model_loader.cc
 *  @brief    PMXファイル読み込みに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/02/17
 *  @version  1.0
 */

 // include
#include "model/model.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>

namespace App
{
  namespace
  {
    // アラインメントを1バイトに設定
#pragma pack(push, 1)
    /**
     *  @brief  Pmxヘッダデータ構造体
     */
    struct Header
    {
      char  signature[4]; ///< シグネチャ
      float version;      ///< バージョン
      char  globalsCount; ///< ファイル全体での設定情報の個数(エンコード方式, 追加UV数等)
      char  globals[8];   ///< ファイル全体での設定情報
    };
    // アラインメントをデフォルトの設定に戻す
#pragma pack(pop)

    /**
     *  @brief  PMXファイルモデル用クラス
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
       *  @brief  ヘッダデータを取得する
       *  @return ヘッダデータ
       */
      const Header& GetHeader() const
      {
        return *header_;
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

        header_.reset();
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
       *  @param  index:追加する頂点座標インデックス
       */
      void AddPointIndex(const uint32_t index)
      {
        point_indices_.emplace_back(index);
      }
    
      /**
       *  @brief  頂点法線ベクトルインデックスを追加する
       *  @param  index:追加する頂点法線ベクトルインデックス
       */
      void AddNormalIndex(const uint32_t index)
      {
        normal_indices_.emplace_back(index);
      }
    
      /**
       *  @brief  頂点テクスチャ座標インデックスを追加する
       *  @param  index:追加する頂点テクスチャ座標インデックス
       */
      void AddTextureCoordIndex(const uint32_t index)
      {
        texture_coord_indices_.emplace_back(index);
      }

      /**
       *  @brief  ヘッダデータを設定する
       *  @param  header:ヘッダデータ
       */
      void AddHeader(Header* header)
      {
        header_.reset(header);
      }
    
    private:
      uint32_t polygon_count_;                        ///< ポリゴン数
      std::vector<DirectX::XMFLOAT3> points_;         ///< 座標リスト
      std::vector<DirectX::XMFLOAT3> normals_;        ///< 法線ベクトルリスト
      std::vector<DirectX::XMFLOAT2> texture_coords_; ///< テクスチャ座標リスト
      std::vector<uint32_t> point_indices_;           ///< 頂点座標インデックスリスト
      std::vector<uint32_t> normal_indices_;          ///< 頂点法線ベクトルインデックスリスト
      std::vector<uint32_t> texture_coord_indices_;   ///< 頂点テクスチャ座標インデックスリスト

      std::unique_ptr<Header> header_;                ///< ヘッダデータ
    };
  };

  /**
   *  @brief  PMXフォーマットファイルからモデルデータを作成する
   *  @param  file_path:読み込みを行うファイルのパス
   *  @return モデルインターフェイスへのポインタ
   */
  std::unique_ptr<IModel> IModel::CreateFromPmx(const std::string& file_path)
  {
    std::unique_ptr<Model> model = nullptr;
    std::unique_ptr<char[]> buffer = nullptr;
    std::uint32_t size = 0;

    // ファイル内容の読み込み
    {
      namespace fs = std::experimental::filesystem;

      // ファイルが存在しない
      if (false == fs::exists(file_path))
      {
        return model;
      }

      std::ifstream ifs(file_path, std::ios::in | std::ios::binary);

      ifs.seekg(0, std::fstream::end);
      unsigned int end = ifs.tellg();

      // 先頭に戻るために一度clear()をかける。
      // これをしないと次のseekg()でコケるときがある。
      ifs.clear();

      ifs.seekg(0, std::fstream::beg);
      size = end - ifs.tellg();

      // バッファの確保
      buffer = std::make_unique<char[]>(size + 1);

      ifs.read(buffer.get(), size);
      ifs.close();
    }

    // モデルデータ読み込み
    {
      auto temporary = buffer.get();

      // ヘッダ読み込み
      {
        // ファイルサイズがヘッダサイズ未満
        if (size < sizeof(Header))
        {
          return model;
        }

        model = std::make_unique<Model>();

        auto header = new Header;
        std::memcpy(header, temporary, sizeof(Header));
        model->AddHeader(header);
        temporary = temporary + sizeof(Header);

        // 違法データチェック
        // シグネチャが「PMX 」でない
        // バージョンが「2.0」か「2.1」でない
      }

      // モデル情報読み込み
      {
        // モデル名
        unsigned int nameSize = 0;
        std::memcpy(&nameSize, temporary, sizeof(nameSize));
        std::string name(temporary + sizeof(nameSize), temporary + sizeof(nameSize) + nameSize);
        temporary = temporary + sizeof(nameSize) + nameSize;

        // 英語のモデル名
        unsigned int englishNameSize = 0;
        std::memcpy(&englishNameSize, temporary, sizeof(englishNameSize));
        std::string englishName(temporary + sizeof(englishNameSize), temporary + sizeof(englishNameSize) + englishNameSize);
        temporary = temporary + sizeof(englishNameSize) + englishNameSize;

        // コメント
        unsigned int commentSize = 0;
        std::memcpy(&commentSize, temporary, sizeof(commentSize));
        std::string comment(temporary + sizeof(commentSize), temporary + sizeof(commentSize) + commentSize);
        temporary = temporary + sizeof(commentSize) + commentSize;

        // 英語のコメント
        unsigned int englishCommentSize = 0;
        std::memcpy(&englishCommentSize, temporary, sizeof(englishCommentSize));
        std::string englishComment(temporary + sizeof(englishCommentSize), temporary + sizeof(englishCommentSize) + englishCommentSize);
        temporary = temporary + sizeof(englishCommentSize) + englishCommentSize;
      }

      // 頂点データ読み込み
      {
        auto vertex_count = 0;
        std::memcpy(&vertex_count, temporary, sizeof(4));
        temporary = temporary + sizeof(4);

        unsigned int addUvCount = model->GetHeader().globals[1];
        unsigned int boneIndexSize = model->GetHeader().globals[5];

        DirectX::XMFLOAT3 point;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 texture_coord;

        for (auto i = 0; i < vertex_count; ++i)
        {
          // 座標
          std::memcpy(&point, temporary, sizeof(DirectX::XMFLOAT3));
          temporary = temporary + sizeof(DirectX::XMFLOAT3);

          // 法線
          std::memcpy(&normal, temporary, sizeof(DirectX::XMFLOAT3));
          temporary = temporary + sizeof(DirectX::XMFLOAT3);

          // UV
          std::memcpy(&texture_coord, temporary, sizeof(DirectX::XMFLOAT2));
          temporary = temporary + sizeof(DirectX::XMFLOAT2);

          // 追加UV
          if (0 < addUvCount)
          {
            temporary = temporary + (sizeof(DirectX::XMFLOAT4) * addUvCount);
          }

          // ウェイト変更方式
          {
            unsigned char weightType = 0;
            std::memcpy(&weightType, temporary, sizeof(weightType));
            temporary = temporary + sizeof(weightType);

            // ウェイト種別毎のデータ読み込み
            // 今回は使用しないのでスキップする
            // TODO:State or Strategyパターン作成
            switch (weightType)
            {
              // BDEF1
            case 0:
              temporary = temporary + boneIndexSize;
              break;
              // BDEF2
            case 1:
              temporary = temporary + (boneIndexSize * 2);
              temporary = temporary + sizeof(float);
              break;
              // BDEF4
            case 2:
              temporary = temporary + (boneIndexSize * 4);
              temporary = temporary + (sizeof(float) * 4);
              break;
            case 3:
              temporary = temporary + (boneIndexSize * 2);
              temporary = temporary + sizeof(float);
              temporary = temporary + (sizeof(DirectX::XMFLOAT3) * 3);
              break;
            default:
              throw "違法なウェイト変更方式データです。";
              break;
            }
          }

          // エッジ倍率
          // 使用しないのでスキップ
          temporary = temporary + sizeof(float);

          model->AddControlPoint(point);
          model->AddNormal(normal);
          model->AddTextureCoord(texture_coord);
        }
      }

      // 頂点インデックスデータ読み込み
      {
        auto index_count_ = 0;
        std::memcpy(&index_count_, temporary, sizeof(4));
        temporary = temporary + sizeof(4);

        unsigned int vertexIndexSize = model->GetHeader().globals[2];
        std::vector<std::uint32_t> indices(index_count_);

        std::memcpy(indices.data(), temporary, vertexIndexSize * index_count_);
        temporary = temporary + (vertexIndexSize * index_count_);

        for (auto index : indices)
        {
          model->AddPointIndex(index);
          model->AddNormalIndex(index);
          model->AddTextureCoordIndex(index);
        }

        auto polygon_count = index_count_ / 3;
        for (auto i = 0; i < polygon_count; ++i)
        {
          model->AddPolygon();
        }
      }

      // テクスチャ読み込み
      {
        unsigned int textureCount = 0;
        std::memcpy(&textureCount, temporary, sizeof(textureCount));
        temporary = temporary + sizeof(textureCount);

        for (unsigned int i = 0; i < textureCount; ++i)
        {
          // テクスチャパスサイズ
          unsigned int texturePathSize = 0;
          std::memcpy(&texturePathSize, temporary, sizeof(texturePathSize));
          temporary = temporary + sizeof(texturePathSize);

          // テクスチャ
          temporary = temporary + texturePathSize;
        }
      }

      // マテリアル読み込み
      {
        unsigned int materialCount = 0;
        std::memcpy(&materialCount, temporary, sizeof(materialCount));
        temporary = temporary + sizeof(materialCount);

        for (unsigned int i = 0; i < materialCount; ++i)
        {
          // マテリアル名
          unsigned int nameSize = 0;
          std::memcpy(&nameSize, temporary, sizeof(nameSize));
          std::string name(temporary + sizeof(nameSize), temporary + sizeof(nameSize) + nameSize);
          temporary = temporary + sizeof(nameSize) + nameSize;

          // マテリアル名(英語)
          unsigned int englishNameSize = 0;
          std::memcpy(&englishNameSize, temporary, sizeof(englishNameSize));
          std::string englishName(temporary + sizeof(englishNameSize), temporary + sizeof(englishNameSize) + englishNameSize);
          temporary = temporary + sizeof(englishNameSize) + englishNameSize;

          // 拡散反射光(Diffuse)
          temporary = temporary + sizeof(DirectX::XMFLOAT4);

          // 鏡面反射光(Specular)
          temporary = temporary + sizeof(DirectX::XMFLOAT3);

          // 鏡面反射の強度
          temporary = temporary + sizeof(float);

          // 環境光(Ambient)
          temporary = temporary + sizeof(DirectX::XMFLOAT3);

          // 描画フラグ
          temporary = temporary + sizeof(unsigned char);

          // エッジ色
          temporary = temporary + sizeof(DirectX::XMFLOAT4);

          // エッジサイズ
          temporary = temporary + sizeof(float);

          // 通常テクスチャ
          temporary = temporary + model->GetHeader().globals[3];

          // スフィアテクスチャ
          temporary = temporary + model->GetHeader().globals[3];

          // スフィアモード
          temporary = temporary + sizeof(unsigned char);

          // 共有Toonフラグ
          bool is_share = false;
          std::memcpy(&is_share, temporary, sizeof(is_share));
          temporary = temporary + sizeof(unsigned char);

          temporary = temporary + (is_share ? sizeof(unsigned char) : model->GetHeader().globals[3]);

          // メモ
          unsigned int memoSize = 0;
          std::memcpy(&memoSize, temporary, sizeof(memoSize));
          std::string memoName(temporary + sizeof(memoSize), temporary + sizeof(memoSize) + memoSize);
          temporary = temporary + sizeof(memoSize) + memoSize;

          // マテリアルに対応する頂点インデックス数
          unsigned int vetexCount = 0;
          std::memcpy(&vetexCount, temporary, sizeof(vetexCount));
          temporary = temporary + sizeof(vetexCount);
        }
      }
    }

    return model;
  }
};
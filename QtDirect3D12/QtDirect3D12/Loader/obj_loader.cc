/**
 *  @file     obj_loader.cc
 *  @brief    OBJファイル読み込みに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/01
 *  @version  1.0
 */

 // include
#include "obj_loader.h"

namespace App
{
  namespace Loader
  {
    namespace Obj
    {
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
          return nullptr;
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
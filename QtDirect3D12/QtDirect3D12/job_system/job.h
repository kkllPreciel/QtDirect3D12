/**
 *  @file     job.h
 *  @brief    ジョブに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/16
 *  @version  1.0
 */

#pragma once

 // include
#include <functional>

namespace App
{
  namespace job_system
  {
    class JobContainer;

    /**
     *  @brief  ジョブクラス
     */
    class Job final
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Job();

      /**
       *  @brief  デストラクタ
       */
      ~Job();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      Job(const Job& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      Job& operator = (const Job& other) = delete;

      /**
       *  @brief  ジョブ実行関数
       *  @param  delta_time:1フレームの経過時間
       */
      void Execute(std::uint64_t delta_time);

      /**
       *  @brief  ジョブコンテナを設定する
       *  @param  job_container:ジョブコンテナへのポインタ
       */
      void SetContainer(JobContainer* job_container);

      /**
       *  @brief  ジョブコンテナを取得する
       *  @return ジョブコンテナのポインタ
       */
      JobContainer* GetContainer() const;

      /**
       *  @brief  ジョブとして実行する関数を設定する
       *  @param  func:関数へのポインタ
       */
      void SetFunction(std::function<void(std::uint64_t)>);

      /**
       *  @brief  ジョブとして実行する関数を設定する
       *  @param  pointer:オブジェクトのポインタ
       *  @param  func:関数へのポインタ
       */
      // template<typename _Class> void SetFunction(_Class* pointer, std::function<void(_Class*, std::uint64_t)>); ///< メンバ関数のみ許容する(std::function)
      // template<typename _Class> void SetFunction(_Class* pointer, void(_Class::* func)(std::uint64_t));         ///< メンバ関数のみ許容する(関数ポインタ)

    private:
      std::function<void(std::uint64_t)> function_; ///< 関数へのポインタ
      JobContainer* container_ = nullptr;           ///< 登録したコンテナへのポインタ
    };

    // templateの実態(メンバ関数のみ許容する関数ポインタ)
    /*template<typename _Class> void Job::SetFunction(_Class* pointer, void(_Class::* func)(std::uint64_t))
    {
      function_ = std::bind(std::mem_fn(func), pointer, std::placeholders::_1);
    }*/
  };
};
/**
 *  @file     async_job.h
 *  @brief    非同期ジョブに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/18
 *  @version  1.0
 */

#pragma once

 // include
#include <functional>

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  非同期ジョブクラス
     */
    class AsyncJob final
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      AsyncJob();

      /**
       *  @brief  デストラクタ
       */
      ~AsyncJob();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      AsyncJob(const AsyncJob& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      AsyncJob& operator = (const AsyncJob& other) = delete;

      /**
       *  @brief  ジョブ実行関数
       */
      void Execute();

      /**
       *  @brief  ジョブとして実行する関数を設定する
       *  @param  func:関数へのポインタ
       */
      void SetFunction(std::function<void()>);

      /**
       *  @brief  ジョブが終了したか?
       *  @return ジョブ終了フラグ
       */
      bool IsFinished() const;

    private:
      std::function<void()> function_;  ///< ジョブ関数へのポインタ
      bool finished_ = false;           ///< 処理終了判定フラグ
    };
  };
};
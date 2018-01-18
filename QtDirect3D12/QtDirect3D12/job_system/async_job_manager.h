/**
 *  @file     async_job_manager.h
 *  @brief    非同期ジョブ管理に関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/18
 *  @version  1.0
 */

#pragma once

 // include
#include <thread>
#include <mutex>
#include <map>

namespace App
{
  namespace job_system
  {
    class AsyncJob;

    /**
     *  @brief  非同期ジョブ管理クラス
     */
    class AsyncJobManager final
    {
    public:
      /**
       *  @brief  非同期処理を実行するスレッド数
       */
      static constexpr std::uint32_t kNumThreads = 3;

      /**
       *  @brief  インスタンスを取得する
       *  @return 非同期ジョブ管理のポインタ
       */
      static std::unique_ptr<AsyncJobManager>& GetInstance()
      {
        return instance_;
      }

      /**
       *  @brief  コンストラクタ
       */
      AsyncJobManager();

      /**
       *  @brief  デストラクタ
       */
      ~AsyncJobManager();

      /**
       *  @brief  初期化を行う
       *  @return 生成成功フラグ
       */
      bool Create();

      /**
       *  @brief  終了処理を行う
       */
      void Destroy();

      /**
       *  @brief  非同期ジョブをキューへ追加する
       *  @param  job:キューへ追加する非同期ジョブ
       *  @param  priority:実行優先度
       */
      void Push(AsyncJob* job, std::uint8_t priority = 128);

      /**
       *  @brief  実行する非同期ジョブをキューから取り出す
       *  @return 実行する非同期ジョブ
       */
      AsyncJob* Pop();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      AsyncJobManager(const AsyncJobManager& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      AsyncJobManager& operator = (const AsyncJobManager& other) = delete;

    private:
      /**
       *  @brief  スレッド実行される関数
       */
      static void Run();

    private:
      static std::unique_ptr<AsyncJobManager> instance_;  ///< 非同期ジョブ管理のインスタンス
      std::thread* threads_[kNumThreads];                 ///< 非同期ジョブを実行するスレッド
      std::mutex mutex_;                                  ///< ジョブキューのスレッド間排他処理用
      std::condition_variable condition_;                 ///< ジョブキューのスレッド間排他処理用
      std::map<std::uint8_t, AsyncJob*> job_queue_;       ///< 非同期実行されるジョブキュー
      bool terminate_;                                    ///< 終了フラグ
    };
  };
};
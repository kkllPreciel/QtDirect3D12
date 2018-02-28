/**
 *  @file     job_container.h
 *  @brief    ジョブコンテナに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/01/16
 *  @version  1.0
 */

#pragma once

 // include
#include <vector>
#include <array>
#include <atomic>
#include <mutex>

namespace App
{
  namespace job_system
  {
    class Job;

    /**
     *  @brief  ジョブコンテナクラス
     */
    class JobContainer final
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      JobContainer();

      /**
       *  @brief  デストラクタ
       */
      ~JobContainer();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      JobContainer(const JobContainer& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      JobContainer& operator = (const JobContainer& other) = delete;

      /**
       *  @brief  実行の初期化を行う
       */
      void MakeReady();

      /**
       *  @brief  ジョブを登録する
       *  @param  job:登録するジョブへのポインタ
       */
      void Register(Job* job);

      /**
       *  @brief  ジョブの登録を解除する
       *  @param  job:登録を解除するジョブへのポインタ
       */
      void Unregister(Job* job);

      /**
       *  @brief  実行するジョブを取得する
       *  @return 実行するジョブへのポインタ
       */
      Job* Pop();

      /**
       *  @brief  終了待ちを行う
       */
      void Wait();

      /**
       *  @brief  終了通知を行う
       */
      void NotifyFinished();

      /**
       *  @brief  処理が終了したか?
       *  @return 処理終了フラグ
       */
      bool IsFinished() const;

      /**
       *  @brief  マルチスレッドで実行するようにする
       */
      void EnableMulti();

      /**
       *  @brief  マルチスレッドで実行するか?
       *  @return マルチスレッド実行フラグ
       */
      bool IsMulti() const;

    private:
      std::atomic<std::uint32_t> current_index_ = 0;  ///< コンテナ内のジョブの実行管理用インデックス
      std::atomic<std::uint32_t> finished_count_ = 0; ///< 終了済み個数
      std::atomic<std::uint32_t> job_size_ = 0;       ///< ジョブの個数
      std::vector<Job*> job_list_;                    ///< ジョブ管理コンテナ
      bool finished_ = false;                         ///< 処理終了フラグ
      bool multi_ = false;                            ///< マルチスレッド実行フラグ
      std::mutex mutex_;                              ///< ミューテックス
      std::condition_variable condition_;             ///< スレッド待機用
      
    };
  };
};
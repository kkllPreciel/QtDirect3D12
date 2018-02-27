/**
 *  @file     job_thread.h
 *  @brief    ジョブスレッドに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/02/27
 *  @version  1.0
 */

#pragma once

 // include
#include <thread>

namespace App
{
  namespace job_system
  {
    class Job;
    class JobQueue;

    /**
     *  @brief  ジョブスレッドクラス
     */
    class JobThread final
    {
      /**
       *  @brief  コンストラクタ
       */
      JobThread();

      /**
       *  @brief  デストラクタ
       */
      ~JobThread();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      JobThread(const JobThread& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      JobThread& operator = (const JobThread& other) = delete;

      /**
       *  @brief  作成する
       *  @param  thread_index:ユニークID
       *  @param  queue:ジョブキュー
       *  @param  run:実行フラグ
       */
      void Create(std::uint32_t thread_index, JobQueue* queue, bool run);

      /**
       *  @brief  終了処理を行う
       */
      void Destroy();

      /**
       *  @brief  ジョブ実行関数
       *  @param  job:実行するジョブ
       */
      void Execute(Job* job);

      static void SetThreadIndex(std::uint32_t index);

      static std::uint32_t GetThreadIndex();

    private:
      /**
       *  @brief  スレッド実行関数
       *  @param  thread_index:ユニークID
       */
      void Run(std::uint32_t thread_index);

      std::thread* thread_ = nullptr;
      JobQueue* queue_ = nullptr;
      Job* current_ = nullptr;

      // TODO:thread_localが使用できないかテストする
      __declspec(thread) static std::uint32_t thread_index_; // ユニークなインデックスを割り振っておく
    };
  };
};
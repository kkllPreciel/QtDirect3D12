/**
 *  @file     async_job.cc
 *  @brief    非同期ジョブに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/18
 *  @version  1.0
 */

 // include
#include "job_system/async_job.h"

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  コンストラクタ
     */
    AsyncJob::AsyncJob()
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    AsyncJob::~AsyncJob()
    {

    }
    
    /**
     *  @brief  ジョブ実行関数
     */
    void AsyncJob::Execute()
    {
      function_();
      finished_ = true;
    }
    
    /**
     *  @brief  ジョブとして実行する関数を設定する
     *  @param  func:関数へのポインタ
     */
    void AsyncJob::SetFunction(std::function<void()> func)
    {
      function_ = func;
    }
    
    /**
     *  @brief  ジョブが終了したか?
     *  @return ジョブ終了フラグ
     */
    bool AsyncJob::IsFinished() const
    {
      return finished_;
    }
  };
};
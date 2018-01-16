/**
 *  @file     job.cc
 *  @brief    ジョブに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/01/16
 *  @version  1.0
 */

 // include
#include "job.h"

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  コンストラクタ
     */
    Job::Job()
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    Job::~Job()
    {

    }
    
    /**
     *  @brief  ジョブ実行関数
     *  @param  delta_time:1フレームの経過時間
     */
    void Job::Execute(std::uint64_t delta_time)
    {
      function_(delta_time);
    }

    /**
     *  @brief  ジョブコンテナを設定する
     *  @param  job_container:ジョブコンテナへのポインタ
     */
    void Job::SetContainer(JobContainer* job_container)
    {
      container_ = job_container;
    }
  };
};
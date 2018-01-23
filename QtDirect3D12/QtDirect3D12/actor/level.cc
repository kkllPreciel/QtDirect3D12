/**
 *  @file     level.cc
 *  @brief    ���x���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/23
 *  @version  1.0
 */

 // include
#include "actor/level.h"
#include "job_system/job_scheduler.h"
#include "job_system/async_job_manager.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    Level::Level()
    {

    }

    /**
     *  @brief  �f�X�g���N�^
     */
    Level::~Level()
    {
      Destroy();
    }

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool Level::Create()
    {
      job_.SetFunction([&](std::uint64_t delta_time) { this->Execute(delta_time); });
      job_system::JobScheduler::GetInstance()->Register(&job_, job_system::JobScheduler::kMoveUpdate);

      async_job_.SetFunction([&]() { this->AsyncExecute(); });
      job_system::AsyncJobManager::GetInstance()->Push(&async_job_);

      return true;
      
    }

    /**
     *  @brief  �X�V�������s��
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void Level::Execute(std::uint64_t delta_time)
    {
      if (initialized_ == false && async_job_.IsFinished())
      {
        initialized_ = true;
      }
    }

    /**
     *  @brief  �I���������s��
     */
    void Level::Destroy()
    {

    }

    /**
     *  @brief  �񓯊����s�����֐�
     */
    void Level::AsyncExecute()
    {
      // �񓯊��ǂݍ��ݏ���
    }
  };
};
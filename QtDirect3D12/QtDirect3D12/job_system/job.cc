/**
 *  @file     job.cc
 *  @brief    �W���u�Ɋւ��鏈�����s���v���O�����\�[�X
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
     *  @brief  �R���X�g���N�^
     */
    Job::Job()
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    Job::~Job()
    {

    }
    
    /**
     *  @brief  �W���u���s�֐�
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void Job::Execute(std::uint64_t delta_time)
    {
      function_(delta_time);
    }
  };
};
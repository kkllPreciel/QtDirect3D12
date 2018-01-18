/**
 *  @file     async_job.cc
 *  @brief    �񓯊��W���u�Ɋւ��鏈�����s���v���O�����\�[�X
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
     *  @brief  �R���X�g���N�^
     */
    AsyncJob::AsyncJob()
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    AsyncJob::~AsyncJob()
    {

    }
    
    /**
     *  @brief  �W���u���s�֐�
     */
    void AsyncJob::Execute()
    {
      function_();
      finished_ = true;
    }
    
    /**
     *  @brief  �W���u�Ƃ��Ď��s����֐���ݒ肷��
     *  @param  func:�֐��ւ̃|�C���^
     */
    void AsyncJob::SetFunction(std::function<void()> func)
    {
      function_ = func;
    }
    
    /**
     *  @brief  �W���u���I��������?
     *  @return �W���u�I���t���O
     */
    bool AsyncJob::IsFinished() const
    {
      return finished_;
    }
  };
};
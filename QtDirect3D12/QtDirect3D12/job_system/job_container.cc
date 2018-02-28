/**
 *  @file     job_container.cc
 *  @brief    �W���u�R���e�i�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/16
 *  @version  1.0
 */

 // include
#include "job_container.h"
#include <cassert>
#include "job_system/job.h"

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    JobContainer::JobContainer()
    {
      // 128���̗̈���m�ۂ���
      job_list_.reserve(128);
    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    JobContainer::~JobContainer()
    {

    }

    /**
     *  @brief  ���s�̏��������s��
     */
    void JobContainer::MakeReady()
    {
      current_index_ = 0;
    }
    
    /**
     *  @brief  �W���u��o�^����
     *  @param  job:�o�^����W���u�ւ̃|�C���^
     */
    void JobContainer::Register(Job* job)
    {
      job_list_.push_back(job);
      job->SetContainer(this);
    }
    
    /**
     *  @brief  �W���u�̓o�^����������
     *  @param  job:�o�^����������W���u�ւ̃|�C���^
     */
    void JobContainer::Unregister(Job* job)
    {
      decltype(auto) end = job_list_.cend();
      for (decltype(auto) it = job_list_.begin(); it != end; ++it)
      {
        if (job == *it)
        {
          job->SetContainer(nullptr);
          job_list_.erase(it);
          break;
        }
      }
    }
    
    /**
     *  @brief  ���s����W���u���擾����
     *  @return ���s����W���u�ւ̃|�C���^
     */
    Job* JobContainer::Pop()
    {
      std::uint32_t index = current_index_++;
      if (job_list_.size() <= index)
      {
        return nullptr;
      }

      return job_list_[index];
    }
  };
};
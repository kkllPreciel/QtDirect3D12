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
      finished_count_ = 0;
      finished_ = job_list_.size() == 0;
    }
    
    /**
     *  @brief  �W���u��o�^����
     *  @param  job:�o�^����W���u�ւ̃|�C���^
     */
    void JobContainer::Register(Job* job)
    {
      job_list_.push_back(job);
      job->SetContainer(this);
      job_size_ = static_cast<std::uint32_t>(job_list_.size());
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

      job_size_ = static_cast<std::uint32_t>(job_list_.size());
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

    /**
     *  @brief  �I���҂����s��
     */
    void JobContainer::Wait()
    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (false == finished_)
      {
        condition_.wait(lock);
      }
    }

    /**
     *  @brief  �I���ʒm���s��
     */
    void JobContainer::NotifyFinished()
    {
      if (++finished_count_ == job_size_)
      {
        std::unique_lock<std::mutex> lock(mutex_);
        finished_ = true;
        condition_.notify_one();
      }
    }
    
    /**
     *  @brief  �������I��������?
     *  @return �����I���t���O
     */
    bool JobContainer::IsFinished() const
    {
      return finished_;
    }
    
    /**
     *  @brief  �}���`�X���b�h�Ŏ��s����悤�ɂ���
     */
    void JobContainer::EnableMulti()
    {
      multi_ = true;
    }
    
    /**
     *  @brief  �}���`�X���b�h�Ŏ��s���邩?
     *  @return �}���`�X���b�h���s�t���O
     */
    bool JobContainer::IsMulti() const
    {
      return multi_;
    }
  };
};
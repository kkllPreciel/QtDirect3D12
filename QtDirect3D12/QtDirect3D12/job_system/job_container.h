/**
 *  @file     job_container.h
 *  @brief    �W���u�R���e�i�Ɋւ���w�b�_�t�@�C��
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
     *  @brief  �W���u�R���e�i�N���X
     */
    class JobContainer final
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      JobContainer();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~JobContainer();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      JobContainer(const JobContainer& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      JobContainer& operator = (const JobContainer& other) = delete;

      /**
       *  @brief  ���s�̏��������s��
       */
      void MakeReady();

      /**
       *  @brief  �W���u��o�^����
       *  @param  job:�o�^����W���u�ւ̃|�C���^
       */
      void Register(Job* job);

      /**
       *  @brief  �W���u�̓o�^����������
       *  @param  job:�o�^����������W���u�ւ̃|�C���^
       */
      void Unregister(Job* job);

      /**
       *  @brief  ���s����W���u���擾����
       *  @return ���s����W���u�ւ̃|�C���^
       */
      Job* Pop();

      /**
       *  @brief  �I���҂����s��
       */
      void Wait();

      /**
       *  @brief  �I���ʒm���s��
       */
      void NotifyFinished();

      /**
       *  @brief  �������I��������?
       *  @return �����I���t���O
       */
      bool IsFinished() const;

      /**
       *  @brief  �}���`�X���b�h�Ŏ��s����悤�ɂ���
       */
      void EnableMulti();

      /**
       *  @brief  �}���`�X���b�h�Ŏ��s���邩?
       *  @return �}���`�X���b�h���s�t���O
       */
      bool IsMulti() const;

    private:
      std::atomic<std::uint32_t> current_index_ = 0;  ///< �R���e�i���̃W���u�̎��s�Ǘ��p�C���f�b�N�X
      std::atomic<std::uint32_t> finished_count_ = 0; ///< �I���ς݌�
      std::atomic<std::uint32_t> job_size_ = 0;       ///< �W���u�̌�
      std::vector<Job*> job_list_;                    ///< �W���u�Ǘ��R���e�i
      bool finished_ = false;                         ///< �����I���t���O
      bool multi_ = false;                            ///< �}���`�X���b�h���s�t���O
      std::mutex mutex_;                              ///< �~���[�e�b�N�X
      std::condition_variable condition_;             ///< �X���b�h�ҋ@�p
      
    };
  };
};
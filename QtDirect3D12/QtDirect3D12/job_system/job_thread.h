/**
 *  @file     job_thread.h
 *  @brief    �W���u�X���b�h�Ɋւ���w�b�_�t�@�C��
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
     *  @brief  �W���u�X���b�h�N���X
     */
    class JobThread final
    {
      /**
       *  @brief  �R���X�g���N�^
       */
      JobThread();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~JobThread();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      JobThread(const JobThread& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      JobThread& operator = (const JobThread& other) = delete;

      /**
       *  @brief  �쐬����
       *  @param  thread_index:���j�[�NID
       *  @param  queue:�W���u�L���[
       *  @param  run:���s�t���O
       */
      void Create(std::uint32_t thread_index, JobQueue* queue, bool run);

      /**
       *  @brief  �I���������s��
       */
      void Destroy();

      /**
       *  @brief  �W���u���s�֐�
       *  @param  job:���s����W���u
       */
      void Execute(Job* job);

      static void SetThreadIndex(std::uint32_t index);

      static std::uint32_t GetThreadIndex();

    private:
      /**
       *  @brief  �X���b�h���s�֐�
       *  @param  thread_index:���j�[�NID
       */
      void Run(std::uint32_t thread_index);

      std::thread* thread_ = nullptr;
      JobQueue* queue_ = nullptr;
      Job* current_ = nullptr;

      // TODO:thread_local���g�p�ł��Ȃ����e�X�g����
      __declspec(thread) static std::uint32_t thread_index_; // ���j�[�N�ȃC���f�b�N�X������U���Ă���
    };
  };
};
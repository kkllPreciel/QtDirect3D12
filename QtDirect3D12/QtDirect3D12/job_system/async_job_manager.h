/**
 *  @file     async_job_manager.h
 *  @brief    �񓯊��W���u�Ǘ��Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/18
 *  @version  1.0
 */

#pragma once

 // include
#include <thread>
#include <mutex>
#include <map>

namespace App
{
  namespace job_system
  {
    class AsyncJob;

    /**
     *  @brief  �񓯊��W���u�Ǘ��N���X
     */
    class AsyncJobManager final
    {
    public:
      /**
       *  @brief  �񓯊����������s����X���b�h��
       */
      static constexpr std::uint32_t kNumThreads = 3;

      /**
       *  @brief  �C���X�^���X���擾����
       *  @return �񓯊��W���u�Ǘ��̃|�C���^
       */
      static std::unique_ptr<AsyncJobManager>& GetInstance()
      {
        return instance_;
      }

      /**
       *  @brief  �R���X�g���N�^
       */
      AsyncJobManager();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~AsyncJobManager();

      /**
       *  @brief  ���������s��
       *  @return ���������t���O
       */
      bool Create();

      /**
       *  @brief  �I���������s��
       */
      void Destroy();

      /**
       *  @brief  �񓯊��W���u���L���[�֒ǉ�����
       *  @param  job:�L���[�֒ǉ�����񓯊��W���u
       *  @param  priority:���s�D��x
       */
      void Push(AsyncJob* job, std::uint8_t priority = 128);

      /**
       *  @brief  ���s����񓯊��W���u���L���[������o��
       *  @return ���s����񓯊��W���u
       */
      AsyncJob* Pop();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      AsyncJobManager(const AsyncJobManager& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      AsyncJobManager& operator = (const AsyncJobManager& other) = delete;

    private:
      /**
       *  @brief  �X���b�h���s�����֐�
       */
      static void Run();

    private:
      static std::unique_ptr<AsyncJobManager> instance_;  ///< �񓯊��W���u�Ǘ��̃C���X�^���X
      std::thread* threads_[kNumThreads];                 ///< �񓯊��W���u�����s����X���b�h
      std::mutex mutex_;                                  ///< �W���u�L���[�̃X���b�h�Ԕr�������p
      std::condition_variable condition_;                 ///< �W���u�L���[�̃X���b�h�Ԕr�������p
      std::map<std::uint8_t, AsyncJob*> job_queue_;       ///< �񓯊����s�����W���u�L���[
      bool terminate_;                                    ///< �I���t���O
    };
  };
};
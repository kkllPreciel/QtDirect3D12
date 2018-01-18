/**
 *  @file     async_job.h
 *  @brief    �񓯊��W���u�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/18
 *  @version  1.0
 */

#pragma once

 // include
#include <functional>

namespace App
{
  namespace job_system
  {
    /**
     *  @brief  �񓯊��W���u�N���X
     */
    class AsyncJob final
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      AsyncJob();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~AsyncJob();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      AsyncJob(const AsyncJob& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      AsyncJob& operator = (const AsyncJob& other) = delete;

      /**
       *  @brief  �W���u���s�֐�
       */
      void Execute();

      /**
       *  @brief  �W���u�Ƃ��Ď��s����֐���ݒ肷��
       *  @param  func:�֐��ւ̃|�C���^
       */
      void SetFunction(std::function<void()>);

      /**
       *  @brief  �W���u���I��������?
       *  @return �W���u�I���t���O
       */
      bool IsFinished() const;

    private:
      std::function<void()> function_;  ///< �W���u�֐��ւ̃|�C���^
      bool finished_ = false;           ///< �����I������t���O
    };
  };
};
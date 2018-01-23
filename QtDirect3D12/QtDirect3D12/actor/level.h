/**
 *  @file     level.h
 *  @brief    ���x���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/23
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include "job_system/job.h"
#include "job_system/async_job.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  ���x���N���X
     */
    class Level final
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Level();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~Level();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      Level(const Level& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      Level& operator = (const Level& other) = delete;

      /**
       *  @brief  ���������s��
       *  @return �����������t���O
       */
      bool Create();

      /**
       *  @brief  �X�V�������s��
       *  @param  delta_time:1�t���[���̌o�ߎ���
       */
      void Execute(std::uint64_t delta_time);

      /**
       *  @brief  �I���������s��
       */
      void Destroy();

    private:
      /**
       *  @brief  �񓯊����s�����֐�
       */
      void AsyncExecute();

    private:
      job_system::Job job_;             ///< �X�V�����p�W���u
      job_system::AsyncJob async_job_;  ///< �񓯊������p�W���u
      bool initialized_ = false;        ///< �������I���t���O
    };
  };
};
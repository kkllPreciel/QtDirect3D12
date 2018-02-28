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

    private:
      std::uint32_t current_index_ = 0; ///< �R���e�i���̃W���u�̎��s�Ǘ��p�C���f�b�N�X
      std::vector<Job*> job_list_;      ///< �W���u�Ǘ��R���e�i
    };
  };
};
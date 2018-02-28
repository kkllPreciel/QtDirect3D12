/**
 *  @file     job.h
 *  @brief    �W���u�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/16
 *  @version  1.0
 */

#pragma once

 // include
#include <functional>

namespace App
{
  namespace job_system
  {
    class JobContainer;

    /**
     *  @brief  �W���u�N���X
     */
    class Job final
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Job();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~Job();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      Job(const Job& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      Job& operator = (const Job& other) = delete;

      /**
       *  @brief  �W���u���s�֐�
       *  @param  delta_time:1�t���[���̌o�ߎ���
       */
      void Execute(std::uint64_t delta_time);

      /**
       *  @brief  �W���u�R���e�i��ݒ肷��
       *  @param  job_container:�W���u�R���e�i�ւ̃|�C���^
       */
      void SetContainer(JobContainer* job_container);

      /**
       *  @brief  �W���u�R���e�i���擾����
       *  @return �W���u�R���e�i�̃|�C���^
       */
      JobContainer* GetContainer() const;

      /**
       *  @brief  �W���u�Ƃ��Ď��s����֐���ݒ肷��
       *  @param  func:�֐��ւ̃|�C���^
       */
      void SetFunction(std::function<void(std::uint64_t)>);

      /**
       *  @brief  �W���u�Ƃ��Ď��s����֐���ݒ肷��
       *  @param  pointer:�I�u�W�F�N�g�̃|�C���^
       *  @param  func:�֐��ւ̃|�C���^
       */
      // template<typename _Class> void SetFunction(_Class* pointer, std::function<void(_Class*, std::uint64_t)>); ///< �����o�֐��̂݋��e����(std::function)
      // template<typename _Class> void SetFunction(_Class* pointer, void(_Class::* func)(std::uint64_t));         ///< �����o�֐��̂݋��e����(�֐��|�C���^)

    private:
      std::function<void(std::uint64_t)> function_; ///< �֐��ւ̃|�C���^
      JobContainer* container_ = nullptr;           ///< �o�^�����R���e�i�ւ̃|�C���^
    };

    // template�̎���(�����o�֐��̂݋��e����֐��|�C���^)
    /*template<typename _Class> void Job::SetFunction(_Class* pointer, void(_Class::* func)(std::uint64_t))
    {
      function_ = std::bind(std::mem_fn(func), pointer, std::placeholders::_1);
    }*/
  };
};
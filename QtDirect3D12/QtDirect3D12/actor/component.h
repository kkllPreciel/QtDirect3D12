/**
 *  @file     component.h
 *  @brief    �R���|�[�l���g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/20
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include "actor/actor.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �R���|�[�l���g�N���X
     */
    class Component
    {
    public:
      /**
       *  @brief  �R���|�[�l���g�̎��ID
       */
      enum ID : std::uint32_t
      {
        kIllegal = 0,
        kMove,        ///< �ړ�����
      };

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~Component();

      /**
       *  @brief  �R���X�g���N�^
       */
      Component() = delete;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      Component(const Component& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      Component& operator = (const Component& other) = delete;

      virtual bool Create() = 0;
      virtual void Destroy() = 0;

    protected:
      /**
       *  @brief  �����t���R���X�g���N�^
       *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
       */
      Component(Actor* owner);

    private:
      Actor* owner_;  ///< �I�[�i�[�A�N�^�[
    };
  };
};
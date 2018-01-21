/**
 *  @file     actor.h
 *  @brief    �A�N�^�[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/20
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include <map>
#include <DirectXMath.h>

namespace App
{
  namespace actor
  {
    class Component;

    /**
     *  @brief  �A�N�^�[�N���X
     */
    class Actor
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Actor();

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~Actor();

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      Actor(const Actor& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      Actor& operator = (const Actor& other) = delete;

      /**
       *  @brief  ���������s��
       *  @return �����������t���O
       */
      bool Create();

      /**
       *  @brief  �I���������s��
       */
      void Destroy();

      /**
       *  @brief  �X�V�������s��
       *  @param  delta_time:1�t���[���̌o�ߎ���
       */
      void Update(std::uint64_t delta_time);

      /**
       *  @brief  ���j�[�NID��ݒ肷��
       *  @param  unique_id:���j�[�NID
       */
      void SetUniqueId(std::uint64_t unique_id);

      /**
       *  @brief  ���j�[�NID���擾����
       *  @return ���j�[�NID
       */
      std::uint64_t GetUniqueId() const;

      /**
       *  @brief  ���W��ݒ肷��
       *  @param  position:�ݒ肷����W
       */
      void SetPosition(DirectX::XMVECTOR position);

      /**
       *  @brief  ���W���擾����
       *  @return ���W
       */
      DirectX::XMVECTOR GetPosition() const;

      /**
       *  @brief  ��]��ݒ肷��
       *  @param  rotation:�ݒ肷���]
       */
      void SetRotation(DirectX::XMVECTOR rotation);

      /**
       *  @brief  ��]���擾����
       *  @return ��]
       */
      DirectX::XMVECTOR GetRotation() const;

      /**
       *  @brief  �R���|�[�l���g���쐬����
       */
      template<typename _Type> _Type* AddComponent();

      /**
       *  @brief  �R���|�[�l���g���擾����
       */
      template<typename _Type> _Type* GetComponent();

    private:
      std::uint64_t unique_id_;                                       ///< �A�N�^�[�Ԃł̃��j�[�N��ID
      std::map<std::uint16_t, Component*> components_;                ///< �R���|�[�l���g�p�R���e�i
      DirectX::XMVECTOR position_ = DirectX::XMVectorZero();          ///< ���W(�x�N�g��)
      DirectX::XMVECTOR rotation_ = DirectX::XMQuaternionIdentity();  ///< ��](�N�H�[�^�j�I��)
    };
  };
};
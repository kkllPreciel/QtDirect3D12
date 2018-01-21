/**
 *  @file     actor.cc
 *  @brief    �A�N�^�[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/20
 *  @version  1.0
 */

 // include
#include "actor/actor.h"
#include <cassert>
#include "job_system/job.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    Actor::Actor()
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    Actor::~Actor()
    {

    }
    
    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool Actor::Create()
    {
      return true;
    }
    
    /**
     *  @brief  �I���������s��
     */
    void Actor::Destroy()
    {

    }
    
    /**
     *  @brief  �X�V�������s��
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void Actor::Update(std::uint64_t delta_time)
    {

    }
    
    /**
     *  @brief  ���j�[�NID��ݒ肷��
     *  @param  unique_id:���j�[�NID
     */
    void Actor::SetUniqueId(std::uint64_t unique_id)
    {
      unique_id_ = unique_id;
    }
    
    /**
     *  @brief  ���j�[�NID���擾����
     *  @return ���j�[�NID
     */
    std::uint64_t Actor::GetUniqueId() const
    {
      return unique_id_;
    }
    
    /**
     *  @brief  ���W��ݒ肷��
     *  @param  position:�ݒ肷����W
     */
    void Actor::SetPosition(DirectX::XMVECTOR position)
    {
      position_ = position;
    }

    /**
     *  @brief  ���W���擾����
     *  @return ���W
     */
    DirectX::XMVECTOR Actor::GetPosition() const
    {
      return position_;
    }
    
    /**
     *  @brief  ��]��ݒ肷��
     *  @param  rotation:�ݒ肷���]
     */
    void Actor::SetRotation(DirectX::XMVECTOR rotation)
    {
      rotation_ = rotation;
    }

    /**
     *  @brief  ��]���擾����
     *  @return ��]
     */
    DirectX::XMVECTOR Actor::GetRotation() const
    {
      return rotation_;
    }

    /**
     *  @brief  �R���|�[�l���g���쐬����
     */
    template<typename _Type> _Type* Actor::AddComponent()
    {
      // �f�o�b�O����p�̃`�F�b�N����
      // �R���|�[�l���g�̓�d�o�^�͕s����
      assert(GetComponent<_Type>() == nullptr);

      _Type* component = new _Type();
      component->Create();
      components_.insert(std::pair<std::uint16_t, Component*>(_Type::GetId(), component));

      return component;
    }
    
    /**
     *  @brief  �R���|�[�l���g���擾����
     */
    template<typename _Type> _Type* Actor::GetComponent()
    {
      std::map<std::uint16_t, Component*>::iterator it = components_.find(_Type::GetId());
      if (it != components_.end())
      {
        it->second->Destroy();
        return static_cast<_Type*>(it->second);
      }

      return nullptr;
    }
  };
};
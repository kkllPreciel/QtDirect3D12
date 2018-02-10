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
#include "actor/component.h"
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
      Destroy();
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
      for (decltype(auto) component : components_)
      {
        component.second->Destroy();
        delete component.second;
      }

      components_.clear();
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
  };
};
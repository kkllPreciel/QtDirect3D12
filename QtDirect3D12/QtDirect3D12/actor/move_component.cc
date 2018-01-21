/**
 *  @file     move_component.cc
 *  @brief    �ړ������p�R���|�[�l���g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/21
 *  @version  1.0
 */

 // include
#include "actor/move_component.h"
#include "actor/actor.h"
#include "job_system/job_scheduler.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �����t���R���X�g���N�^
     *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
     */
    MoveComponent::MoveComponent(Actor* owner) : Component(owner)
    {

    }
    
    /**
     *  @brief  �f�X�g���N�^
     */
    MoveComponent::~MoveComponent()
    {

    }
    
    /**
     *  @brief  �R���|�[�l���g�̎��ID���擾����
     *  @return �R���|�[�l���g�̎��ID
     */
    constexpr Component::ID MoveComponent::GetId()
    {
      return Component::ID::kMove;
    }
    

    /**
     *  @brief  ���������s��
     *  @return �����������t���O
     */
    bool MoveComponent::Create()
    {
      // TODO:����������

      return true;
    }
    
    /**
     *  @brief  �I���������s��
     */
    void MoveComponent::Destroy()
    {

    }

    /**
     *  @brief  �X�V�������s��
     *  @param  delta_time:1�t���[���̌o�ߎ���
     */
    void MoveComponent::Update(std::uint64_t delta_time)
    {
      // TODO:�X�V����
    }
    
    /**
     *  @brief  �ړ�������ݒ肷��
     *  @param  velocity:�ړ�����
     */
    void MoveComponent::SetVelocity(DirectX::XMVECTOR velocity)
    {
      velocity_ = velocity;
    }
    
    /**
     *  @brief  �ړ��������擾����
     *  @return �ړ�����
     */
    DirectX::XMVECTOR MoveComponent::GetVelocity() const
    {
      return velocity_;
    }
    
    /**
     *  @brief  ���x��ݒ肷��
     *  @param  speed:���x
     */
    void MoveComponent::SetSpeed(float speed)
    {
      speed_ = speed;
    }
    
    /**
     *  @brief  ���x���擾����
     *  @return ���x
     */
    float MoveComponent::GetSpeed() const
    {
      return speed_;
    }
  };
};
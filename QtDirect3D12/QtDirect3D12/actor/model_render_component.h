/**
 *  @file     model_render_component.h
 *  @brief    ���f���`�揈���p�R���|�[�l���g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/02/14
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <DirectXMath.h>
#include "actor/component.h"
#include "job_system/job.h"
#include "model/app_model.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  ���f���`�揈���p�̃R���|�[�l���g�N���X
     */
    class ModelRenderComponent : public Component
    {
    public:
      /**
       *  @brief  �����t���R���X�g���N�^
       *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
       */
      ModelRenderComponent(Actor* owner);

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~ModelRenderComponent();

      /**
       *  @brief  �R���X�g���N�^
       */
      ModelRenderComponent() = delete;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      ModelRenderComponent(const ModelRenderComponent& other) = delete;
      
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      ModelRenderComponent& operator = (const ModelRenderComponent& other) = delete;

      /**
       *  @brief  �R���|�[�l���g�̎��ID���擾����
       *  @return �R���|�[�l���g�̎��ID
       */
      static constexpr std::uint32_t GetId()
      {
        return Component::ID::kRender;
      }

      /**
       *  @brief  ���������s��
       *  @return �����������t���O
       */
      virtual bool Create() override;

      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() override;

      /**
       *  @brief  �X�V�������s��
       *  @param  delta_time:1�t���[���̌o�ߎ���
       */
      void Update(std::uint64_t delta_time);

      /**
       *  @brief  ���f����ݒ肷��
       *  @param  model:���f��
       */
      void SetModel(std::shared_ptr<App::IAppModel> model);

      /**
       *  @brief  �v���~�e�B�u�̃g�|���W�[��ݒ肷��
       *  @param  topology:�g�|���W�[
       */
      void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

    private:
      std::unique_ptr<job_system::Job> job_;  ///< �W���u
      std::shared_ptr<App::IAppModel> model_; ///< ���f��
      D3D12_PRIMITIVE_TOPOLOGY topology_;     ///< �v���~�e�B�u�̃^�C�v
    };
  };
};
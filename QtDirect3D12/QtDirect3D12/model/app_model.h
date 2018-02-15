/**
 *  @file     app_model.h
 *  @brief    �A�v���P�[�V�����Ŏg�p���郂�f���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/02/15
 *  @version  1.0
 */

#pragma once

 // include
#include "model/model.h"

namespace App
{
  /**
   *  @brief  ���f���\���C���^�[�t�F�C�X
   */
  class IAppModel : public IModel
  {
  public:
    // �A���C�������g��1�o�C�g�ɐݒ�
#pragma pack(push, 1)
    struct Vertex
    {
      DirectX::XMFLOAT3 position; ///< ���W
      DirectX::XMFLOAT3 normal;   ///< �@���x�N�g��
      DirectX::XMFLOAT2 texcoord; ///< �e�N�X�`��UV���W
    };
#pragma pack(pop)

  public:
    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IAppModel() {};

    /**
     *  @brief  ���_���X�g���擾����
     *  @return ���_�̃��X�g
     */
    virtual const std::vector<Vertex>& GetVertices() const = 0;

    /**
     *  @brief  ���_�C���f�b�N�X���X�g���擾����
     *  @return ���_�C���f�b�N�X�̃��X�g
     */
    virtual const std::vector<uint32_t>& GetIndices() const = 0;

    /**
     *  @brief  OBJ�t�H�[�}�b�g�t�@�C�����烂�f���f�[�^���쐬����
     *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
     *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
     */
    static std::unique_ptr<IAppModel> LoadFromObj(const std::string& file_path);
  };
};
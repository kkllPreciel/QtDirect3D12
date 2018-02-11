/**
 *  @file     model.h
 *  @brief    ���f���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/12/31
 *  @version  1.0
 */

#pragma once

 // include
#include <cstdint>
#include <vector>
#include <memory>
#include <DirectXMath.h>

namespace App
{
  /**
   *  @brief  ���f���\���C���^�[�t�F�C�X
   */
  class IModel
  {
  public:
    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~IModel() {};

    /**
     *  @brief  ���_���W���X�g���擾����
     *  @return ���_���W�̃��X�g
     */
    virtual const std::vector<DirectX::XMFLOAT3>& GetControlPoints() const = 0;

    /**
     *  @brief  ���_���W�����擾����
     *  @return ���_���W��
     */
    virtual uint32_t GetControlPointCount() const = 0;

    /**
     *  @brief  ���_�@���x�N�g�����X�g���擾����
     *  @return ���_�@���x�N�g���̃��X�g
     */
    virtual const std::vector<DirectX::XMFLOAT3>& GetNormals() const = 0;

    /**
     *  @brief  ���_�@���x�N�g�������擾����
     *  @return ���_�@���x�N�g����
     */
    virtual uint32_t GetNormalCount() const = 0;

    /**
     *  @brief  ���_�e�N�X�`�����W���X�g���擾����
     *  @return ���_�e�N�X�`�����W�̃��X�g
     */
    virtual const std::vector<DirectX::XMFLOAT2>& GetTextureCoords() const = 0;

    /**
     *  @brief  �e�N�X�`�����W�����擾����
     *  @return �e�N�X�`�����W��
     */
    virtual uint32_t GetTextureCoordCount() const = 0;

    /**
     *  @brief  �|���S�������擾����
     *  @return �|���S����
     */
    virtual uint32_t GetPolygonCount() const = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�f�[�^���擾����
     *  @return ���_�C���f�b�N�X�f�[�^�̃��X�g
     */
    virtual const std::vector<uint32_t>& GetIndices() const = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�����擾����
     *  @return ���_�C���f�b�N�X��
     */
    virtual uint32_t GetIndexCount() const = 0;

    /**
     *  @brief  ���_�@���x�N�g���C���f�b�N�X���X�g���擾����
     *  @return ���_�@���x�N�g���C���f�b�N�X�̃��X�g
     */
    virtual const std::vector<uint32_t>& GetNormalIndices() const = 0;

    /**
     *  @brief  ���_�@���x�N�g���C���f�b�N�X�����擾����
     *  @return ���_�@���x�N�g���C���f�b�N�X��
     */
    virtual uint32_t GetNormalIndexCount() const = 0;

    /**
     *  @brief  ���_�e�N�X�`�����W�C���f�b�N�X���X�g���擾����
     *  @return ���_�e�N�X�`�����W�C���f�b�N�X�̃��X�g
     */
    virtual const std::vector<uint32_t>& GetTextureCoordIndices() const = 0;

    /**
     *  @brief  ���_�e�N�X�`�����W�C���f�b�N�X�����擾����
     *  @return ���_�e�N�X�`�����W�C���f�b�N�X��
     */
    virtual uint32_t GetTextureCoordIndexCount() const = 0;

    /**
     *  @brief  �f�[�^���J������
     */
    virtual void Release() = 0;

    /**
     *  @brief  OBJ�t�H�[�}�b�g�t�@�C�����烂�f���f�[�^���쐬����
     *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
     *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
     */
    static std::unique_ptr<IModel> CreateFromObj(const std::string& file_path);
  };
};
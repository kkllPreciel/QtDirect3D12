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
     *  @brief  ���_���W�f�[�^���擾����
     *  @return ���_���W�f�[�^�̃��X�g
     */
    virtual const std::vector<DirectX::XMFLOAT4A>& GetVertices() const = 0;

    /**
     *  @brief  ���_�����擾����
     *  @return ���_��
     */
    virtual std::size_t GetVertexCount() const = 0;

    /**
     *  @brief  �|���S�������擾����
     *  @return �|���S����
     */
    virtual std::size_t GetPolygonCount() const = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�f�[�^���擾����
     *  @return ���_�C���f�b�N�X�f�[�^�̃��X�g
     */
    virtual const std::vector<uint32_t>& GetIndices() const = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�����擾����
     *  @return ���_�C���f�b�N�X��
     */
    virtual std::size_t GetIndexCount() const = 0;

    /**
     *  @brief  �f�[�^���J������
     */
    virtual void Release() = 0;
  };
};
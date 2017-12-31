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
     *  @brief  ���_�����擾����
     *  @return ���_��
     */
    virtual uint32_t GetVertexCount() = 0;

    /**
     *  @brief  �|���S�������擾����
     *  @return �|���S����
     */
    virtual uint32_t GetPolygonCount() = 0;

    /**
     *  @brief  ���_�C���f�b�N�X�����擾����
     *  @return ���_�C���f�b�N�X��
     */
    virtual uint32_t GetIndexCount() = 0;

    /**
     *  @brief  �f�[�^���J������
     */
    virtual void Release() = 0;
  };
};
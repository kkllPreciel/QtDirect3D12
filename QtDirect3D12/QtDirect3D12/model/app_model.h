/**
 *  @file     app_model.h
 *  @brief    �A�v���P�[�V�����Ŏg�p���郂�f���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/02/15
 *  @version  1.0
 */

#pragma once

 // include
#include <Sein/Direct3D12/vertex_buffer.h>
#include <Sein/Direct3D12/index_buffer.h>
#include <Sein/Direct3D12/direct3d12_device.h>
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
     *  @brief  ���_�o�b�t�@���擾����
     *  @return ���_�o�b�t�@
     */
    virtual const Sein::Direct3D12::VertexBuffer& GetVertexBuffer() const = 0;

    /**
     *  @brief  �C���f�b�N�X�o�b�t�@���擾����
     *  @return �C���f�b�N�X�o�b�t�@
     */
    virtual const Sein::Direct3D12::IndexBuffer& GetIndexBuffer() const = 0;

    /**
     *  @brief  OBJ�t�H�[�}�b�g�t�@�C�����烂�f���f�[�^���쐬����
     *  @param  device:�f�o�C�X
     *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
     *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
     */
    static std::unique_ptr<IAppModel> LoadFromObj(const Sein::Direct3D12::Device& device, const std::string& file_path);

    /**
     *  @brief  PMX�t�H�[�}�b�g�t�@�C�����烂�f���f�[�^���쐬����
     *  @param  device:�f�o�C�X
     *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
     *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
     */
    static std::unique_ptr<IAppModel> LoadFromPmx(const Sein::Direct3D12::Device& device, const std::string& file_path);
  };
};
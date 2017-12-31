/**
 *  @file     loader.h
 *  @brief    �ǂݍ��݂Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/12/28
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <string>
#include "model.h"

namespace App
{
  /**
   *  @brief  �ǂݍ��݃C���^�[�t�F�C�X
   */
  class ILoader
  {
  public:
    /**
     *  @brief  �X�e�[�^�X�R�[�h
     */
    enum StatusCode
    {
      SUCCESS,        ///< ����
      FILE_NOT_FOUND, ///< �t�@�C�������݂��Ȃ�
      UNSUPPORTED,    ///< ���Ή��̃t�H�[�}�b�g
    };

    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~ILoader() {};

    /**
     *  @brief  �ǂݍ��݂��s��
     *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
     *  @param  status_code:�ǂݍ��݌��ʂ̃X�e�[�^�X�R�[�h
     *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
     */
    virtual std::unique_ptr<IModel> Load(const std::string& file_path, StatusCode* const status_code) = 0;
  };
};
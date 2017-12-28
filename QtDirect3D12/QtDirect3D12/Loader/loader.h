/**
 *  @file     loader.h
 *  @brief    �ǂݍ��݂Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/12/28
 *  @version  1.0
 */

#pragma once

 // include
#include <string>

namespace App
{
  /**
   *  @brief  �ǂݍ��݃C���^�[�t�F�C�X
   */
  class ILoader
  {
  public:
    /**
     *  @brief  �f�X�g���N�^
     */
    virtual ~ILoader() {};

    /**
     *  @brief  �ǂݍ��݂��s��
     *  @param  filePath:�ǂݍ��݂��s���t�@�C���̃p�X
     */
    virtual void Load(std::string filePath) = 0;

    /**
     *  @brief  �f�[�^���J������
     */
    virtual void Release() = 0;
  };
};
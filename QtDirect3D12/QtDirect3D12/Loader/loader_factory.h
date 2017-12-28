/**
 *  @file     loader_factory.h
 *  @brief    �ǂݍ��݃N���X�����Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/12/29
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include "loader.h"

namespace App
{
  /**
   *  @brief  �ǂݍ��݃N���X�����p�N���X
   */
  class LoaderFactory
  {
  public:
    /**
     *  @brief  �R���X�g���N�^
     */
    LoaderFactory();

    /**
     *  @brief  �f�X�g���N�^
     */
    ~LoaderFactory();

    /**
     *  @brief  �ǂݍ��݃N���X�𐶐�����
     *  @return �ǂݍ��݃N���X�̃I�u�W�F�N�g
     */
    std::unique_ptr<ILoader> Create();
  };
};
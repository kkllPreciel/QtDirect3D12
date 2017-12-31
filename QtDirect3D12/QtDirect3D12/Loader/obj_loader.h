/**
 *  @file     obj_loader.h
 *  @brief    OBJ�t�@�C���ǂݍ��݂Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/01/01
 *  @version  1.0
 */

#pragma once

 // include
#include "loader.h"

namespace App
{
  namespace Loader
  {
    namespace Obj
    {
      /**
       *  @brief  OBJ�t�@�C���ǂݍ��ݗp�C���X�^���X�𐶐�����
       *  @return �ǂݍ��ݗp�C���^�[�t�F�C�X
       */
      std::unique_ptr<ILoader> CreateLoader();
    };
  };
};
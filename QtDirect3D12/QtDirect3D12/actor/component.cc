/**
 *  @file     component.cc
 *  @brief    �R���|�[�l���g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/20
 *  @version  1.0
 */

 // include
#include "actor/component.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  �����t���R���X�g���N�^
     *  @param  owner:�R���|�[�l���g���������Ă���I�[�i�[
     */
    Component::Component(Actor* owner) : owner_(owner)
    {

    }

    /**
     *  @brief  �f�X�g���N�^
     */
    Component::~Component()
    {

    }
  };
};
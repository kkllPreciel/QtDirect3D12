/**
 *  @file     component.cc
 *  @brief    コンポーネントに関する処理を行うプログラムソース
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
     *  @brief  引数付きコンストラクタ
     *  @param  owner:コンポーネントを所持しているオーナー
     */
    Component::Component(Actor* owner) : owner_(owner)
    {

    }

    /**
     *  @brief  デストラクタ
     */
    Component::~Component()
    {

    }
  };
};
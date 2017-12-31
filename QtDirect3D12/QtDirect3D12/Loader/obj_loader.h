/**
 *  @file     obj_loader.h
 *  @brief    OBJファイル読み込みに関するヘッダファイル
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
       *  @brief  OBJファイル読み込み用インスタンスを生成する
       *  @return 読み込み用インターフェイス
       */
      std::unique_ptr<ILoader> CreateLoader();
    };
  };
};
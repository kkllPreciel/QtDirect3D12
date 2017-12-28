/**
 *  @file     loader.h
 *  @brief    読み込みに関するヘッダファイル
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
   *  @brief  読み込みインターフェイス
   */
  class ILoader
  {
  public:
    /**
     *  @brief  デストラクタ
     */
    virtual ~ILoader() {};

    /**
     *  @brief  読み込みを行う
     *  @param  filePath:読み込みを行うファイルのパス
     */
    virtual void Load(std::string filePath) = 0;

    /**
     *  @brief  データを開放する
     */
    virtual void Release() = 0;
  };
};
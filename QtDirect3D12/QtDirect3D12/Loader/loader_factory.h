/**
 *  @file     loader_factory.h
 *  @brief    読み込みクラス生成に関するヘッダファイル
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
   *  @brief  読み込みクラス生成用クラス
   */
  class LoaderFactory
  {
  public:
    /**
     *  @brief  コンストラクタ
     */
    LoaderFactory();

    /**
     *  @brief  デストラクタ
     */
    ~LoaderFactory();

    /**
     *  @brief  読み込みクラスを生成する
     *  @return 読み込みクラスのオブジェクト
     */
    std::unique_ptr<ILoader> Create();
  };
};
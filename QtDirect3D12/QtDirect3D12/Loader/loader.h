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
     *  @brief  ステータスコード
     */
    enum StatusCode
    {
      SUCCESS,        ///< 成功
      FILE_NOT_FOUND, ///< ファイルが存在しない
      UNSUPPORTED,    ///< 未対応のフォーマット
    };

    /**
     *  @brief  デストラクタ
     */
    virtual ~ILoader() {};

    /**
     *  @brief  読み込みを行う
     *  @param  filePath:読み込みを行うファイルのパス
     *  @return ステータスコード
     */
    virtual StatusCode Load(std::string filePath) = 0;

    /**
     *  @brief  データを開放する
     */
    virtual void Release() = 0;
  };
};
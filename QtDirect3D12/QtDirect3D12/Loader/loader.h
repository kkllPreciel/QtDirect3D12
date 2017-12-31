/**
 *  @file     loader.h
 *  @brief    読み込みに関するヘッダファイル
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
     *  @param  file_path:読み込みを行うファイルのパス
     *  @param  status_code:読み込み結果のステータスコード
     *  @return モデルインターフェイスへのポインタ
     */
    virtual std::unique_ptr<IModel> Load(const std::string& file_path, StatusCode* const status_code) = 0;
  };
};
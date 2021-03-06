/**
 *  @file     model_render_component.h
 *  @brief    モデル描画処理用コンポーネントに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/02/14
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <DirectXMath.h>
#include "actor/component.h"
#include "job_system/job.h"
#include "model/app_model.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  モデル描画処理用のコンポーネントクラス
     */
    class ModelRenderComponent : public Component
    {
    public:
      /**
       *  @brief  引数付きコンストラクタ
       *  @param  owner:コンポーネントを所持しているオーナー
       */
      ModelRenderComponent(Actor* owner);

      /**
       *  @brief  デストラクタ
       */
      virtual ~ModelRenderComponent();

      /**
       *  @brief  コンストラクタ
       */
      ModelRenderComponent() = delete;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      ModelRenderComponent(const ModelRenderComponent& other) = delete;
      
      /**
       *  @brief  コピー代入演算子オペレータ
       *  @param  other:コピー代入元のインスタンス
       *  @return コピー代入後のインスタンス
       */
      ModelRenderComponent& operator = (const ModelRenderComponent& other) = delete;

      /**
       *  @brief  コンポーネントの種別IDを取得する
       *  @return コンポーネントの種別ID
       */
      static constexpr std::uint32_t GetId()
      {
        return Component::ID::kRender;
      }

      /**
       *  @brief  初期化を行う
       *  @return 初期化成功フラグ
       */
      virtual bool Create() override;

      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() override;

      /**
       *  @brief  更新処理を行う
       *  @param  delta_time:1フレームの経過時間
       */
      void Update(std::uint64_t delta_time);

      /**
       *  @brief  モデルを設定する
       *  @param  model:モデル
       */
      void SetModel(std::shared_ptr<App::IAppModel> model);

      /**
       *  @brief  プリミティブのトポロジーを設定する
       *  @param  topology:トポロジー
       */
      void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

    private:
      std::unique_ptr<job_system::Job> job_;  ///< ジョブ
      std::shared_ptr<App::IAppModel> model_; ///< モデル
      D3D12_PRIMITIVE_TOPOLOGY topology_;     ///< プリミティブのタイプ
    };
  };
};
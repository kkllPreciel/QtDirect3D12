/**
 *  @file     model_render_component.cc
 *  @brief    モデル描画処理用コンポーネントに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/02/14
 *  @version  1.0
 */

 // include
#include "actor/model_render_component.h"
#include "job_system/job_scheduler.h"

namespace App
{
  namespace actor
  {
    /**
     *  @brief  引数付きコンストラクタ
     *  @param  owner:コンポーネントを所持しているオーナー
     */
    ModelRenderComponent::ModelRenderComponent(Actor* owner) : Component(owner)
    {

    }

    /**
     *  @brief  デストラクタ
     */
    ModelRenderComponent::~ModelRenderComponent()
    {
      Destroy();
    }

    /**
     *  @brief  初期化を行う
     *  @return 初期化成功フラグ
     */
    bool ModelRenderComponent::Create()
    {
      Destroy();

      // ジョブに登録する
      job_ = std::make_unique<job_system::Job>();

      job_->SetFunction([&](std::uint64_t delta_time) {
        this->Update(delta_time);
      });
      job_system::JobScheduler::GetInstance()->Register(job_.get(), job_system::JobScheduler::kRenderUpdate);

      return true;
    }
    
    /**
     *  @brief  終了処理を行う
     */
    void ModelRenderComponent::Destroy()
    {
      // ジョブスケジューラへの登録を解除する
      if (job_)
      {
        job_system::JobScheduler::GetInstance()->Unregister(job_.get());
      }

      job_.reset();
    }
    
    /**
     *  @brief  更新処理を行う
     *  @param  delta_time:1フレームの経過時間
     */
    void ModelRenderComponent::Update(std::uint64_t delta_time)
    {
      // 描画するなら行列を更新する
      // オーナーの座標、向き、スケーリングから行列を生成する
      // モデルと必要な情報をレンダリングキューに登録する
    }
    
    /**
     *  @brief  モデルを設定する
     *  @param  model:モデル
     */
    void ModelRenderComponent::SetModel(std::shared_ptr<App::IModel> model)
    {
      model_ = model;
    }
  };
};
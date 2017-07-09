/**
 *	@file		Fence.cpp
 *	@brief		フェンスに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/07/05
 *	@version	1.0
 */

#include "Fence.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		Fence::Fence() : fence(nullptr), index(0), eventHandle(nullptr)
		{

		}

		/**
		 *	@brief	作成する
		 *	@param	device:Direct3D12のデバイス
		 */
		void Fence::Create(ID3D12Device* const device)
		{
			if (FAILED(device->CreateFence(
				index,					// フェンスの初期値
				D3D12_FENCE_FLAG_NONE,	// オプションの指定(今回は指定なし)
				IID_PPV_ARGS(&fence))))
			{
				throw "フェンスの生成に失敗しました。";
			}

			++index;
			// 同期待ち用のイベントを生成
			if (nullptr == (eventHandle = CreateEvent(nullptr, false, false, nullptr)))
			{
				throw "イベントの生成に失敗しました。";
				// throw HRESULT_FROM_WIN32(GetLastError());
			}
		}

		/**
		 *	@brief	待機する
		 *	@param	commandQueue:コマンドキュー
		 */
		void Fence::Wait(ID3D12CommandQueue* const commandQueue)
		{
			// 実行されているコマンドリストが完了したら
			// フェンスに指定の値を設定するようにする
			if (FAILED(commandQueue->Signal(fence, index)))
			{
				throw "コマンドキューのシグナル設定に失敗しました。";
			}

			// 既に処理が終わっている場合は実行しない
			if (fence->GetCompletedValue() < index)
			{
				// フェンスの値が設定した値になったら
				// 指定したイベントを発行させる
				if (FAILED(fence->SetEventOnCompletion(index, eventHandle)))
				{
					throw "フェンスのイベント発行設定に失敗しました。";
				}

				// シグナル状態(イベント発行)になるまで待機する
				WaitForSingleObjectEx(eventHandle, INFINITE, false);
			}

			// フェンスの値を更新する
			++index;
		}
		
		/**
		 *	@brief	デストラクタ
		 */
		Fence::~Fence()
		{
			Release();
		}
		
		/**
		 *	@brief	リソースを開放する
		 */
		void Fence::Release()
		{
			if (nullptr != eventHandle)
			{
				CloseHandle(eventHandle);
				eventHandle = nullptr;
			}

			if (nullptr != fence)
			{
				fence->Release();
				fence = nullptr;
			}
		}
	};
};
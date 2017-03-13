/**
 *	@file		Direct3D12Device.cpp
 *	@brief		Direct3D12デバイスに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/02/07
 *	@version	1.0
 */

#if _DEBUG
#include <sstream>
#endif
#include <winerror.h>
#include "Direct3D12Device.h"

namespace Sein
{
	namespace Direct3D12
	{
		/** 
		 *	@brief	コンストラクタ
		 */
		Device::Device() : device(nullptr), swapChain(nullptr)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		Device::~Device()
		{
			Release();
		}

		/**
		 *	@brief	デバイスを生成する
		 *	@param	handle:ウィンドウハンドル
		 *	@param	width:横幅
		 *	@param	height:縦幅
		 */
		void Device::Create(HWND handle, unsigned int width, unsigned int height)
		{
#if _DEBUG
			// デバッグレイヤーを有効に設定する
			Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
			{
				throw "デバッグレイヤーの生成に失敗しました。";	
			}
			debugInterface->EnableDebugLayer();
#endif
			// ファクトリの生成
			// アダプターの列挙に使用
			Microsoft::WRL::ComPtr<IDXGIFactory4> factory;

			if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
			{
				throw "DXGIファクトリの生成に失敗しました。";
			}

			// Direct3D12のデバイスを作成する
			Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;

			// アダプター(ビデオカード)を列挙する
			for (auto i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pAdapter); ++i)
			{
				DXGI_ADAPTER_DESC1 desc;
				pAdapter->GetDesc1(&desc);

				// TODO:調査
				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					continue;
				}

				// デバイスを作成
				if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
				{
					continue;
				}

#if _DEBUG
				std::wostringstream ostr;
				pAdapter->GetDesc1(&desc);
				ostr << "D3D12-capable hardware found:" << desc.Description << "(" << (desc.DedicatedVideoMemory >> 20) << "MB)\n";
				OutputDebugString(ostr.str().c_str());
#endif
			}
			
			// WARPで生成する
			if (nullptr == device)
			{
#if _DEBUG
				OutputDebugString(L"WARP software adapter requested.  Initializing...\n");
#endif
				if (FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter))))
				{
					throw "WARPアダプターが存在しません。";
				}

				if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
				{
					throw "デバイスの生成に失敗しました。";
				}
			}

			// コマンドキューの作成
			// コマンドキューはGPUへ描画命令が出されたコマンドリストを
			// 順次実行する
			// つまり特定のアダプター(GPU)に関連付けられている
			{
				D3D12_COMMAND_QUEUE_DESC queueDesc = {};
				queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// タイムアウト処理を有効にする
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// レンダリング関連のコマンドリスト

				if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue))))
				{
					throw "コマンドキューの生成に失敗しました。";
				}
			}

			// スワップチェインの作成
			// コマンドキューを指定して作成する = アダプターを指定して作成する
			{
				DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
				swapChainDesc.Width = width;									// ウィンドウ横幅
				swapChainDesc.Height = height;									// ウィンドウ縦幅
				swapChainDesc.BufferCount = 2;									// バッファの数は2個(フロントバッファも含むらしいが、公式サンプル等を見るとバックバッファの数な気がする)
				swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 恐らくバッファのフォーマット(4 成分、32 ビット符号なし整数)
				swapChainDesc.Scaling = DXGI_SCALING_NONE;						// 画面サイズとバッファサイズが等しくない時の拡縮動作(拡大縮小は行わない)
				swapChainDesc.SampleDesc.Quality = 0;							// マルチサンプリングの品質レベル
				swapChainDesc.SampleDesc.Count = 1;								// ピクセル単位のマルチサンプリング数
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バックバッファの使用目的及びCPUアクセスオプション(レンダーターゲットとして使用)
				swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// スワップチェインの動作オプション(モード切替可能に設定)
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// フロントバッファとバックバッファのスワップ挙動指定(バックバッファがディスプレイに表示されたら破棄する)

				Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwapChain;
				if (FAILED(factory->CreateSwapChainForHwnd(
					commandQueue,	// コマンドキュー
					handle,			// ウィンドウハンドル
					&swapChainDesc,	// スワップチェインの設定情報
					nullptr,		// フルスクリーンスワップチェインの設定(ウィンドウモードで作成するのでnullptr)
					nullptr,		// TODO:調査
					&pSwapChain)))
				{
					throw "スワップチェインの生成に失敗しました。";
				}
				
				if (FAILED(pSwapChain.Get()->QueryInterface(IID_PPV_ARGS(&swapChain))))
				{
					throw "IDXGISwapChain3の生成に失敗しました。";
				}
			}

			// 描画コマンド関連
			// コマンドアロケータ生成

			// 描画対象・管理
			// フェンス生成
			// ディスクリプターヒープの作成
			// レンダーターゲットビューの作成
		}

		/**
		 *	@brief	デバイスを開放する
		 */
		void Device::Release()
		{

		}

		/**
		 *	@brief	画面を更新する
		 */
		void Device::Present()
		{

		}
	};
};
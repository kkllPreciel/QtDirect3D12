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
#include <d3dcompiler.h>
#include "Direct3D12Device.h"
#include "DepthStencilView.h"
#include "Fence.h"
#include "Sein/Direct3D12/VertexBuffer.h"
#include "Sein/Direct3D12/IndexBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/** 
		 *	@brief	コンストラクタ
		 */
		Device::Device() :
			device(nullptr), swapChain(nullptr), commandQueue(nullptr), commandAllocator(nullptr),
			commandList(nullptr), descriptorHeap(nullptr), descriptorSize(0), bufferIndex(0),
			rootSignature(nullptr), pipelineState(nullptr), cbvHeap(nullptr), constantBuffer(nullptr),
			constantBufferDataBegin(nullptr), depthStencilView(nullptr), fence(nullptr)
		{
			for (auto i = 0; i < FrameCount; ++i)
			{
				renderTargetList[i] = nullptr;
			}
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
			{
#if 1
				// デバイスを生成
				// アダプターを列挙し作成すると
				// 環境によってメモリリークが発生したため
				// デフォルトのアダプターを使用し作成する
				if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
				{
					throw "デバイスの生成に失敗しました。";
				}
#else
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
#endif
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
				swapChainDesc.BufferCount = FrameCount;							// バッファの数は2個(フロントバッファも含むらしいが、公式サンプル等を見るとバックバッファの数な気がする)
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

				// バックバッファの番号を取得する
				bufferIndex = swapChain->GetCurrentBackBufferIndex();
			}

			// コマンドアロケーターの生成
			// コマンドに使用するバッファ領域を確保する物
			{
				if (FAILED(device->CreateCommandAllocator(
					D3D12_COMMAND_LIST_TYPE_DIRECT,	// コマンドアロケーターの種別(レンダリング関連のコマンドリストを設定)
					IID_PPV_ARGS(&commandAllocator))))
				{
					throw "コマンドアロケーターの生成に失敗しました。";
				}
			}

			// コマンドリストの生成
			// コマンドキューに渡すコマンドのリスト
			{
				if (FAILED(device->CreateCommandList(
					0,									// マルチアダプター(マルチGPU)の場合に使用するアダプター(GPU)の識別子(単一なので0)
					D3D12_COMMAND_LIST_TYPE_DIRECT,		// コマンドリストの種別(レンダリング関連のコマンドリスト)
					commandAllocator,					// このコマンドリストで使用するコマンドアロケーター
					nullptr,							// コマンドリストの初期パイプライン状態(ダミーの初期パイプラインを指定)
					IID_PPV_ARGS(&commandList))))
				{
					throw "コマンドリストの生成に失敗しました。";
				}

				// 記録を終了する
				commandList->Close();
			}

			// Alt + Enterでフルスクリーン化の機能を無効に設定
			factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER);

			// ディスクリプターヒープの作成
			// ディスクリプターはバッファの情報データ(テクスチャバッファ、頂点バッファ等)
			{
				D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
				rtvHeapDesc.NumDescriptors = FrameCount;				// ディスクリプターヒープ内のディスクリプター数(フロントバッファ、バックバッファ)
				rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// 種別はレンダーターゲットビュー
				rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// シェーダーから参照しない

				if (FAILED(device->CreateDescriptorHeap(
					&rtvHeapDesc,	// ディスクリプターヒープの設定情報
					IID_PPV_ARGS(&descriptorHeap
					))))
				{
					throw "ディスクリプターヒープの生成に失敗しました。";
				}

				// レンダーターゲット分のディスクリプターのサイズを取得する
				descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			}

			// ディスクリプターの登録
			{
				D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

				// フレームバッファ数文登録する
				for (auto i = 0; i < FrameCount; ++i)
				{
					if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargetList[i]))))
					{
						throw "バックバッファの取得に失敗しました。";
					}

					// レンダーターゲットビューを作成する
					device->CreateRenderTargetView(
						renderTargetList[i],	// レンダー ターゲットを表すID3D12Resourceへのポインタ
						nullptr,				// D3D12_RENDER_TARGET_VIEW_DESCへのポインタ
						renderTargetViewHandle
					);

					renderTargetViewHandle.ptr += descriptorSize;
				}
			}

			// フェンスの生成
			// Directx12ではGPUの描画の終了待ちを自動で行わない(同期が取れず画面がおかしくなる)
			// そのため同期を取るためのオブジェクト(フェンス)を作成する
			{
				fence = new Fence;
				fence->Create(device);

				// 描画処理を行っている可能性があるので描画終了待ちを行う
				WaitForGpu();
			}

			// アセットを読み込む
			{
				LoadAssets(width, height);
			}
		}

		/**
		 *	@brief	デバイスを開放する
		 */
		void Device::Release()
		{
			// GPUの描画終了待ちを行う
			WaitForGpu();
			
			// フェンスの削除
			if (nullptr != fence)
			{
				fence->Release();
				delete fence;
				fence = nullptr;
			}

			pipelineState->Release();
			depthStencilView->Release();
			delete depthStencilView;
			depthStencilView = nullptr;
			constantBufferDataBegin = nullptr;
			constantBuffer->Unmap(0, nullptr);
			constantBuffer->Release();
			rootSignature->Release();

			for (auto i = 0; i < FrameCount; ++i)
			{
				renderTargetList[i]->Release();
			}

			cbvHeap->Release();
			descriptorHeap->Release();
			commandList->Release();
			commandAllocator->Release();
			commandQueue->Release();
			swapChain->Release();
			device->Release();
		}

		/**
		 *	@brief	シーンを開始する
		 */
		void Device::BeginScene()
		{
			// コマンドアロケーターをリセット
			if (FAILED(commandAllocator->Reset()))
			{
				throw "コマンドアロケーターのリセットに失敗しました。";
			}

			// コマンドリストをリセット
			if (FAILED(commandList->Reset(commandAllocator, nullptr)))
			{
				throw "コマンドリストのリセットに失敗しました。";
			}

			// バックバッファが描画ターゲットとして使用できるようになるまで待つ
			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// バリアはリソースの状態遷移に対して設置
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = renderTargetList[bufferIndex];			// リソースは描画ターゲット
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;			// 遷移前はPresent
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		// 遷移後は描画ターゲット
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			commandList->ResourceBarrier(1, &barrier);

			// バックバッファを描画ターゲットとして設定する
			// デバイスへ深度ステンシルビューをバインドする
			D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			handle.ptr += bufferIndex * descriptorSize;
			D3D12_CPU_DESCRIPTOR_HANDLE depthHandle = depthStencilView->GetDescriptorHandle();
			commandList->OMSetRenderTargets(1, &handle, false, &depthHandle);

			// バックバッファをクリアする
			const float Color[] = { 0.0f, 0.0f, 0.6f, 1.0f };
			commandList->ClearRenderTargetView(handle, Color, 0, nullptr);

			// 深度ステンシルビューをクリアする(深度バッファのみ)
			commandList->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		}

		/**
		 *	@brief	シーンを終了する
		 */
		void Device::EndScene()
		{
			// バックバッファの描画完了を待つためのバリアを設置
			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// バリアはリソースの状態遷移に対して設置
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = renderTargetList[bufferIndex];			// リソースは描画ターゲット
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// 遷移前は描画ターゲット
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// 遷移後はPresent
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			commandList->ResourceBarrier(1, &barrier);

			// コマンドリストをクローズする
			commandList->Close();
		}

		/**
		 *	@brief	画面を更新する
		 */
		void Device::Present()
		{
			// コマンドリストの実行
			ID3D12CommandList* ppCommandLists[] = { commandList };
			commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			// 描画終了待ちを行う
			WaitForGpu();

			// 画面の更新
			if (FAILED(swapChain->Present(1, 0)))
			{
				throw "画面の更新に失敗しました。";
			}

			// バッファ番号を更新
			bufferIndex = swapChain->GetCurrentBackBufferIndex();
		}

		// 後々別クラスへ移動する
#pragma region Fence

		/**
		 *	@brief	描画終了待ちを行う
		 */
		void Device::WaitForGpu()
		{
			fence->Wait(commandQueue);
		}

#pragma endregion

		/**
		 *	@brief	アセットを読み込む
		 *	@param	width:ウィンドウ横幅
		 *	@param	height:ウィンドウ縦幅
		 */
		void Device::LoadAssets(unsigned int width, unsigned int height)
		{
			// 定数バッファの生成
			{
				CreateConstantBuffer();
			}

			// 深度ステンシルビューの作成
			{
				CreateDepthStencilView(width, height);
			}

			// ルートシグネチャの作成
			{
				// ディスクリプターレンジの設定
				D3D12_DESCRIPTOR_RANGE descriptorRange;
				descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;								// ディスクリプターの種別(定数バッファビュー)
				descriptorRange.NumDescriptors = 1;															// ディスクリプターの数
				descriptorRange.BaseShaderRegister = 0;														// 範囲内のベースシェーダレジスタ
				descriptorRange.RegisterSpace = 0;															// レジスタ空間(TODO:調べる)
				descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// ルートシグネチャ開始からのディスクリプタのオフセット?

				// ルートパラメータの設定
				D3D12_ROOT_PARAMETER rootParameter;
				rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;				// ルートシグネチャのスロットの種別(ディスクリプタテーブル)
				rootParameter.DescriptorTable.NumDescriptorRanges = 1;									// ディスクリプターレンジの数
				rootParameter.DescriptorTable.pDescriptorRanges = &descriptorRange;						// ディスクリプターレンジのポインタ(数が1超なら配列の先頭ポインタ)
				rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;						// ルートシグネチャのスロットの内容にアクセスできるシェーダの種別(頂点シェーダのみ)

				// ルートシグネチャの設定
				D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
				rootSignatureDesc.NumParameters = 1;													// ルートシグネチャのスロット数
				rootSignatureDesc.pParameters = &rootParameter;											// スロットの構造?
				rootSignatureDesc.NumStaticSamplers = 0;												// 静的サンプラー数
				rootSignatureDesc.pStaticSamplers = nullptr;											// 静的サンプラー設定データのポインタ
				rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT	// オプション(描画に使用する)
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS							// ハルシェーダからルートシグネチャへのアクセス禁止
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS							// ドメインシェーダからルートシグネチャへのアクセス禁止
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS						// ジオメトリシェーダからルートシグネチャへのアクセス禁止
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;							// ピクセルシェーダからルートシグネチャへのアクセス禁止
				

				// ルートシグネチャのシリアル化
				Microsoft::WRL::ComPtr<ID3DBlob> signature;
				if (FAILED(D3D12SerializeRootSignature(
					&rootSignatureDesc,				// ルートシグネチャの設定
					D3D_ROOT_SIGNATURE_VERSION_1,	// ルートシグネチャのバージョン
					&signature,						// シリアライズしたルートシグネチャへアクセスするためのインターフェイス(ポインタ)
					nullptr							// シリアライザのエラーメッセージへアクセスするためのインターフェイス(ポインタ)
				)))
				{
					throw "ルートシグネチャのシリアライズに失敗しました。";
				}

				// ルートシグネチャの生成
				if (FAILED(device->CreateRootSignature(
					0,									// マルチアダプター(マルチGPU)の場合に使用するアダプター(GPU)の識別子(単一なので0)
					signature->GetBufferPointer(),		// シリアル化されたシグネチャ設定へのポインタ
					signature->GetBufferSize(),			// メモリのブロックサイズ
					IID_PPV_ARGS(&rootSignature))))
				{
					throw "ルートシグネチャの生成に失敗しました。";
				}
			}

			// パイプラインステートの作成
			// シェーダーも一緒にコンパイルする
			// 後々はコンパイル済みのシェーダーを使用する
			{
				Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
				Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

				// コンパイルオプションフラグを設定する
#if defined(_DEBUG)
				UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
				UINT compileFlags = 0;
#endif

				// 頂点シェーダーのコンパイル
				if (FAILED(D3DCompileFromFile(
					L"shaders.hlsl",	// シェーダーファイル名
					nullptr,			// シェーダーマクロ(今回は使用しない)
					nullptr,			// インクルードファイルを取り扱うために使用するID3DIncludeインタフェースへのポインタ(今回は使用しない)
					"VSMain",			// エントリーポイントの関数名
					"vs_5_0",			// コンパイルターゲット(今回は頂点シェーダーでシェーダーモデル5)
					compileFlags,		// コンパイルオプション
					0,					// エフェクトファイルのコンパイルオプション(今回はエフェクトとして使用しないので0)
					&vertexShader,		// コンパイルされたコードへアクセスするためのID3DBlobインタフェースのポインタ
					nullptr				// コンパイルエラーメッセージへアクセスするためのID3DBlobインタフェースのポインタ
				)))
				{
					throw "頂点シェーダーのコンパイルに失敗しました。";
				}

				// ピクセルシェーダーのコンパイル
				if (FAILED(D3DCompileFromFile(
					L"shaders.hlsl",	// シェーダーファイル名
					nullptr,			// シェーダーマクロ(今回は使用しない)
					nullptr,			// インクルードファイルを取り扱うために使用するID3DIncludeインタフェースへのポインタ(今回は使用しない)
					"PSMain",			// エントリーポイントの関数名
					"ps_5_0",			// コンパイルターゲット(今回はピクセルシェーダーでシェーダーモデル5)
					compileFlags,		// コンパイルオプション
					0,					// エフェクトファイルのコンパイルオプション(今回はエフェクトとして使用しないので0)
					&pixelShader,		// コンパイルされたコードへアクセスするためのID3DBlobインタフェースのポインタ
					nullptr				// コンパイルエラーメッセージへアクセスするためのID3DBlobインタフェースのポインタ
				)))
				{
					throw "ピクセルシェーダーのコンパイルに失敗しました。";
				}

				// 頂点入力レイアウト
				D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
				{
					// 頂点座標
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

					// 頂点の法線
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

					// テクスチャ座標
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
				};

				// ラスタライザーステートの設定
				D3D12_RASTERIZER_DESC rasterizer_desc;
				rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;								// 三角形を描画するときに使用する塗りつぶしモード(今回は塗りつぶし)
				rasterizer_desc.CullMode = D3D12_CULL_MODE_NONE;								// カリングのモード(裏向きのポリゴンを描画しない)
				rasterizer_desc.FrontCounterClockwise = FALSE;									// ポリゴンの表裏の判定方法(今回は時計回りなら表)
				rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;							// 深度バイアス(Z-Fighting対策?)
				rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;				// 深度バイアスのクランプ値
				rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;	// 傾斜を考慮した深度バイアス(ピーターパン現象対策?)
				rasterizer_desc.DepthClipEnable = TRUE;											// Zクリッピングを行うか(今回は行う)
				rasterizer_desc.MultisampleEnable = FALSE;										// マルチサンプリングのアンチエイリアシングを行うか(今回はしない)
				rasterizer_desc.AntialiasedLineEnable = FALSE;									// 線のアンチエイリアシングを行うか(今回はしない)
				rasterizer_desc.ForcedSampleCount = 0;											// UAVレンダリングまたはラスタライズ中に強制されるサンプル数(今回は強制しない)
				rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;	// 保守的なラスタライズフラグ(今回はオフ)
				
				// パイプラインステートの設定に使用する頂点シェーダーのデータ構造を作成
				D3D12_SHADER_BYTECODE vs;
				vs.pShaderBytecode = vertexShader.Get()->GetBufferPointer();
				vs.BytecodeLength = vertexShader.Get()->GetBufferSize();

				// パイプラインステートの設定に使用するピクセルシェーダーのデータ構造を作成
				D3D12_SHADER_BYTECODE ps;
				ps.pShaderBytecode = pixelShader.Get()->GetBufferPointer();
				ps.BytecodeLength = pixelShader.Get()->GetBufferSize();

				// レンダーターゲットのブレンド状態の設定
				const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTarggetBlendDesc = 
				{
					false,							// ブレンディングを有効にするか(今回は無効)
					false,							// 論理演算を有効にするか(今回は無効)
					D3D12_BLEND_ONE,				// RGB値ピクセルシェーダ出力に対して実行する操作を指定
					D3D12_BLEND_ZERO,				// レンダーターゲットの現在のRGB値に対して実行する動作を指定する
					D3D12_BLEND_OP_ADD,				// RGBまたはアルファブレンディング操作を指定(ソース1とソース2を追加)
					D3D12_BLEND_ONE,				// ピクセルシェーダ出力するアルファ値に対して実行する動作を指定
					D3D12_BLEND_ZERO,				// レンダーターゲットで現在のアルファ値に対して実行する動作
					D3D12_BLEND_OP_ADD,				// RBGまたはアルファブレンディング操作を指定(ソース1とソース2を追加)
					D3D12_LOGIC_OP_NOOP,			// レンダーターゲットに設定する論理演算?(ノーオペレーション、何もしない)
					D3D12_COLOR_WRITE_ENABLE_ALL	// 
				};

				// ブレンド状態の設定
				D3D12_BLEND_DESC blendDesc;
				blendDesc.AlphaToCoverageEnable = false;	// アルファトゥカバレッジを有効にするか
				blendDesc.IndependentBlendEnable = false;	// 同時レンダーターゲットに独立したブレンドを有効にするかどうかを指定します
				for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
					blendDesc.RenderTarget[i] = defaultRenderTarggetBlendDesc;

				// 深度ステンシル状態の設定
				D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
				depthStencilDesc.DepthEnable = true;							// デプステストを有効にするか?
				depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 深度ステンシル バッファーへの書き込みをオンにします
				depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		// 深度データを既存の深度データと比較する関数です。ソースデータが対象データよりも小さい場合、比較に合格します。
				depthStencilDesc.StencilEnable = false;							// ステンシルテストを有効にするか?

				// マルチサンプリングパラメーターの設定
				DXGI_SAMPLE_DESC sampleDesc = {};
				sampleDesc.Count = 1;	// ピクセル単位のマルチサンプリングの数

				// パイプラインステートの設定
				D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
				psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };	// 入力レイアウトの構造
				psoDesc.pRootSignature = rootSignature;										// ルートシグネチャ
				psoDesc.RasterizerState = rasterizer_desc;									// ラスタライザの状態
				psoDesc.VS = vs;															// 頂点シェーダーの構造
				psoDesc.PS = ps;															// ピクセルシェーダーの構造
				psoDesc.BlendState = blendDesc;												// ブレンド状態の構造
				psoDesc.DepthStencilState = depthStencilDesc;								// 深度ステンシル状態の構造
				psoDesc.SampleMask = UINT_MAX;												// ブレンドの状態のためのサンプルのマスク
				psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;		// 入力プリミティブ(三角形)
				psoDesc.NumRenderTargets = 1;												// レンダーターゲットのフォーマット数
				psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;							// レンダーターゲットのフォーマット
				psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;									// 深度ステンシルのフォーマット
				psoDesc.SampleDesc = sampleDesc;											// サンプリング状態の構造

				// グラフィックスパイプラインステートの生成
				if (FAILED(device->CreateGraphicsPipelineState(
					&psoDesc,
					IID_PPV_ARGS(&pipelineState))))
				{
					throw "パイプラインステートの生成に失敗しました。";
				}
			}
		}

		/**
		 *	@brief	描画する
		 *	@param	vertexBuffer:頂点バッファ
		 *	@param	indexBuffer:頂点インデックスバッファ
		 */
		void Device::Render(const VertexBuffer& vertebBuffer, const IndexBuffer& indexBuffer)
		{
			static float now = 0.0f;
			static float angle = DirectX::XM_PI / 180.0f;

			// 回転
			now += angle;

			// ワールド行列を更新
			DirectX::XMStoreFloat4x4(&(constantBufferData.world), DirectX::XMMatrixRotationY(now));

			// ビュー行列を作成
			DirectX::XMVECTORF32 eye = { 0.0f, 10.0f, -30.5f, 0.0f };
			DirectX::XMVECTORF32 at = { 0.0f, 10.0f, 0.0f, 0.0f };
			DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
			DirectX::XMStoreFloat4x4(&(constantBufferData.view), DirectX::XMMatrixLookAtLH(eye, at, up));

			// プロジェクション行列を作成
			DirectX::XMStoreFloat4x4(&(constantBufferData.projection), DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 3.0f, 600.0f / 400.0f, 0.1f, 1000.0f));

			// 定数バッファを更新
			std::memcpy(constantBufferDataBegin, &constantBufferData, sizeof(ConstantBuffer));

			// ビューポートの作成
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = 600;
			viewport.Height = 400;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;

			// シザー矩形(シザーテスト)の作成
			D3D12_RECT scissor;
			scissor.left = 0;
			scissor.top = 0;
			scissor.right = 600;
			scissor.bottom = 400;

			// パイプラインステートの設定(切り替えない場合は、コマンドリストリセット時に設定可能)
			commandList->SetPipelineState(pipelineState);

			// グラフィックスパイプラインのルートシグネチャを設定する
			commandList->SetGraphicsRootSignature(rootSignature);

			// 描画に使用するディスクリプターヒープを設定
			commandList->SetDescriptorHeaps(1, &cbvHeap);

			// ビューポートの設定
			commandList->RSSetViewports(1, &viewport);

			// シザー矩形(シザーテスト)の設定
			commandList->RSSetScissorRects(1, &scissor);

			// プリミティブトポロジーの設定
			//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
			commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// 頂点バッファビューの設定
			commandList->IASetVertexBuffers(0, 1, &(vertebBuffer.GetView()));

			// 頂点インデックスビューの設定
			commandList->IASetIndexBuffer(&(indexBuffer.GetView()));

			// ディスクリプータヒープテーブルを設定
			commandList->SetGraphicsRootDescriptorTable(0, cbvHeap->GetGPUDescriptorHandleForHeapStart());

			// 描画コマンドの生成
			// TODO:頂点インデックスを使用して描画する
			commandList->DrawIndexedInstanced(321567, 1, 0, 0, 0);
			//commandList->DrawInstanced(88402, 1, 0, 0);
			//commandList->DrawInstanced(3, 1, 0, 0);
		}

		/**
		 *	@brief	デバイスを取得する
		 *	@return	デバイスへの参照
		 */
		ID3D12Device& Device::GetDevice() const
		{
			return *device;
		}

		// 後々別クラスへ移動する
#pragma region ConstantBuffer

		/**
		 *	@brief	定数をバッファを作成する
		 */
		void Device::CreateConstantBuffer()
		{
			// 定数バッファ用ディスクリプターヒープを生成
			{
				D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
				cbvHeapDesc.NumDescriptors = 1;									// ディスクリプターヒープ内のディスクリプター数(定数バッファ)
				cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;		// 定数バッファ or シェーダーリソース(テクスチャ) or ランダムアクセス のどれかのヒープ
				cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// シェーダーからアクセス可

				if (device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&cbvHeap)))
				{
					throw "定数バッファ用ディスクリプターヒープの生成に失敗しました。";
				}
			}

			// 定数バッファを生成
			{
				// ヒープの設定
				D3D12_HEAP_PROPERTIES properties;
				properties.Type = D3D12_HEAP_TYPE_UPLOAD;						// ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
				properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// CPUページプロパティ(不明に設定)
				properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;	// ヒープのメモリプール(不明に設定)
				properties.CreationNodeMask = 1;								// 恐らくヒープが生成されるアダプター(GPU)の番号
				properties.VisibleNodeMask = 1;									// 恐らくヒープが表示されるアダプター(GPU)の番号

				// リソースの設定
				D3D12_RESOURCE_DESC resource_desc;
				resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;		// リソースの種別(今回はバッファ)
				resource_desc.Alignment = 0;									// アラインメント
				resource_desc.Width = sizeof(ConstantBuffer);					// リソースの幅(今回は定数バッファのサイズ)
				resource_desc.Height = 1;										// リソースの高さ(今回は定数バッファ分の幅を確保しているので1)
				resource_desc.DepthOrArraySize = 1;								// リソースの深さ(テクスチャ等に使用する物、今回は1)
				resource_desc.MipLevels = 1;									// ミップマップのレベル(今回は1)
				resource_desc.Format = DXGI_FORMAT_UNKNOWN;						// リソースデータフォーマット(R8G8B8A8等)(今回は不明)
				resource_desc.SampleDesc.Count = 1;								// ピクセル単位のマルチサンプリング数
				resource_desc.SampleDesc.Quality = 0;							// マルチサンプリングの品質レベル
				resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;			// テクスチャレイアウトオプション
				resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;					// リソース操作オプションフラグ(今回は無し)

				// 定数バッファ用リソースの生成(ヒープも同時に生成される)
				if (FAILED(device->CreateCommittedResource(
					&properties,						// ヒープの設定
					D3D12_HEAP_FLAG_NONE,				// ヒープオプション(設定なし)
					&resource_desc,						// リソースの設定
					D3D12_RESOURCE_STATE_GENERIC_READ,	// リソースの状態
					nullptr,							// クリアカラーのデフォルト値
					IID_PPV_ARGS(&constantBuffer))))
				{
					throw "定数バッファ用リソースの作成に失敗しました。";
				}

				// 定数バッファビューの設定
				D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
				constantBufferViewDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();		// バッファのアドレス
				constantBufferViewDesc.SizeInBytes = (sizeof(ConstantBuffer) + 255) & ~255;			// 定数バッファは256バイトでアラインメントされていなければならない

				// 定数バッファビューを生成
				device->CreateConstantBufferView(&constantBufferViewDesc, cbvHeap->GetCPUDescriptorHandleForHeapStart());

				// マップ。アプリケーション終了までアンマップしない
				if (FAILED(constantBuffer->Map(
					0,									// サブリソースのインデックス番号
					nullptr,							// CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
					reinterpret_cast<void**>(&constantBufferDataBegin))))	// リソースデータへのポインタ
				{
					throw "定数バッファ用リソースへのポインタの取得に失敗しました。";
				}

				// 定数バッファデータの初期化
				auto aspect = static_cast<float>(600) / static_cast<float>(400);
				DirectX::XMStoreFloat4x4(&(constantBufferData.world), DirectX::XMMatrixIdentity());
				DirectX::XMStoreFloat4x4(&(constantBufferData.view), DirectX::XMMatrixIdentity());
				DirectX::XMStoreFloat4x4(&(constantBufferData.projection), DirectX::XMMatrixIdentity());
				std::memcpy(constantBufferDataBegin, &constantBufferData, sizeof(ConstantBuffer));
			}
		}
#pragma endregion

		// 深度ステンシルビュー関連
#pragma region DepthStencliView
		/**
		 *	@brief	深度ステンシルビューを作成する
		 *	@param	width:ウィンドウ横幅
		 *	@param	height:ウィンドウ縦幅
		 */
		void Device::CreateDepthStencilView(unsigned int width, unsigned int height)
		{
			depthStencilView = new DepthStencilView();
			depthStencilView->Create(device, width, height);
		}
#pragma endregion
	};
};
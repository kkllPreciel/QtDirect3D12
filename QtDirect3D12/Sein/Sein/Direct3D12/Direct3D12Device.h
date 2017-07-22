/**
 *	@file		Direct3D12Device.h
 *	@brief		Direct3D12のデバイスに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/02/06
 *	@version	1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include <wrl\client.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>

namespace Sein
{
	namespace Direct3D12
	{
		class VertexBuffer;
		class IndexBuffer;
		class DepthStencilView;
		class Fence;
		class ConstantBuffer;
		class ShaderResourceBuffer;

		/**
		 *	@brief	Direct3D12用デバイスクラス(スワップチェインも一緒)
		 */
		class Device
		{
		public:
			/** 
			 *	@brief	コンストラクタ
			 */
			Device();

			/**
			 *	@brief	デストラクタ
			 */
			~Device();

			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			Device(const Device& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			Device& operator = (const Device& other) = delete;

			/**
			 *	@brief	デバイスを生成する
			 *	@param	handle:ウィンドウハンドル
			 *	@param	width:横幅
			 *	@param	height:縦幅
			 */
			void Create(HWND handle, unsigned int width, unsigned int height);

			/**
			 *	@brief	デバイスを開放する
			 */
			void Release();

			/**
			 *	@brief	シーンを開始する
			 */
			void BeginScene();

			/**
			 *	@brief	シーンを終了する
			 */
			void EndScene();

			/**
			 *	@brief	画面を更新する
			 */
			void Present();

			/**
			 *	@brief	デバイスを取得する
			 *	@return	デバイスへの参照
			 */
			ID3D12Device& GetDevice() const;

		private:
			ID3D12Device*				device;					///< デバイス
			IDXGISwapChain3*			swapChain;				///< スワップチェイン
			ID3D12CommandQueue*			commandQueue;			///< コマンドキュー
			ID3D12CommandAllocator*		commandAllocator;		///< コマンドアロケーター
			ID3D12GraphicsCommandList*	commandList;			///< コマンドリスト
			ID3D12DescriptorHeap*		descriptorHeap;			///< ディスクリプターヒープ
			unsigned int				descriptorSize;			///< ディスクリプターのサイズ

			// 後々ダブルバッファクラスへ移動
			static const unsigned int	FrameCount = 2;					///< フレーム数(ダブルバッファ)
			ID3D12Resource*				renderTargetList[FrameCount];	///< レンダーターゲットのリスト
			unsigned int				bufferIndex;					///< 現在のバッファ番号

			Fence*						fence;		///< フェンス

			/**
			 *	@brief	描画終了待ちを行う
			 */
			void WaitForGpu();

			/**
			 *	@brief	アセットを読み込む
			 *	@param	width:ウィンドウ横幅
			 *	@param	height:ウィンドウ縦幅
			 */
			void LoadAssets(unsigned int width, unsigned int height);

			// ルートシグネチャ関連
			// 後々別クラスへ移動
#pragma region RootSignature

			ID3D12RootSignature*	rootSignature;	///< ルートシグネチャ

#pragma endregion

			// パイプラインステート関連
			// 後々別クラスへ移動
#pragma region PipelineState
			ID3D12PipelineState*	pipelineState;	///< パイプラインステート

		public:
			/**
			 *	@brief	描画する
			 *	@param	vertexBuffer:頂点バッファ
			 *	@param	indexBuffer:頂点インデックスバッファ
			 */
			void Render(const VertexBuffer& vertebBuffer, const IndexBuffer& indexBuffer);
#pragma endregion

			// 定数バッファ関連
			// 後々別クラスへ移動
#pragma region ConstantBuffer

			/**
			 *	@brief	定数バッファ用構造体
			 */
			struct ConstantBufferType
			{
				DirectX::XMFLOAT4X4 world;						///< ワールド行列(世界空間)
				DirectX::XMFLOAT4X4 view;						///< ビュー行列(視線空間)
				DirectX::XMFLOAT4X4 projection;					///< プロジェクション行列(射影空間)
			};

			std::unique_ptr<ID3D12DescriptorHeap>	cbvSrvHeap;	///< 定数バッファビュー、シェーダーリソースビュー用ディスクリプターヒープ
			std::unique_ptr<ConstantBuffer>			cbvBuffer;	///< 定数バッファ
			ConstantBufferType constantBufferData;				///< 定数バッファ用のデータ

		private:
			/**
			 *	@brief	定数バッファを作成する
			 */
			void CreateConstantBuffer();

#pragma endregion

			// 深度ステンシルビュー関連
#pragma region DepthStencilView
		private:
			DepthStencilView* depthStencilView;	///< 深度ステンシルビュー

			/**
			 *	@brief	深度ステンシルビューを作成する
			 *	@param	width:ウィンドウ横幅
			 *	@param	height:ウィンドウ縦幅
			 */
			void CreateDepthStencilView(unsigned int width, unsigned int height);


#pragma endregion

			// インスタンシング関連
			// 後々別クラスへ移動
#pragma region Instancing
		private:
			/**
			 *	@brief	インスタンシング用構造体
			 */
			struct InstanceBuffer
			{
				DirectX::XMFLOAT4X4 world;			///< ワールド行列(世界空間)
			};

			const unsigned int INSTANCE_NUM = 5;								///< インスタンスの数
			std::vector<InstanceBuffer>				instanceBufferData;			///< 各インスタンス毎のデータリスト
			std::unique_ptr<ShaderResourceBuffer>	srBuffer;					///< シェーダーリソースバッファ

		private:
			/**
			 *	@brief	インスタンスバッファを作成する
			 */
			void CreateInstanceBuffer();

#pragma endregion
		};
	};
};
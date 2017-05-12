/**
 *	@file		VertexBuffer.cpp
 *	@brief		頂点バッファに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/05/09
 *	@version	1.0
 */

// include
#include <cstring>
#include "VertexBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		VertexBuffer::VertexBuffer() : buffer(nullptr)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		VertexBuffer::~VertexBuffer()
		{
			if (nullptr != buffer)
			{
				buffer->Release();
				buffer = nullptr;
			}
		}

		/**
		 *	@brief	頂点バッファの生成を行う
		 *	@param	device:Direct3D12のデバイス
		 *	@param	size:頂点バッファのサイズ(全頂点合計サイズ)
		 *	@param	stride:1頂点のサイズ
		 *	@param	vertices:頂点データ
		 */
		void VertexBuffer::Create(ID3D12Device* const device, const unsigned int size, const unsigned short stride, const void* const vertices)
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
			resource_desc.Width = size;										// リソースの幅(今回は頂点バッファのサイズ)
			resource_desc.Height = 1;										// リソースの高さ(今回は頂点バッファ分の幅を確保しているので1)
			resource_desc.DepthOrArraySize = 1;								// リソースの深さ(テクスチャ等に使用する物、今回は1)
			resource_desc.MipLevels = 1;									// ミップマップのレベル(今回は1)
			resource_desc.Format = DXGI_FORMAT_UNKNOWN;						// リソースデータフォーマット(R8G8B8A8等)(今回は不明)
			resource_desc.SampleDesc.Count = 1;								// ピクセル単位のマルチサンプリング数
			resource_desc.SampleDesc.Quality = 0;							// マルチサンプリングの品質レベル
			resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;			// テクスチャレイアウトオプション
			resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;					// リソース操作オプションフラグ(今回は無し)

			// リソースの生成(ヒープも同時に生成される)
			if (FAILED(device->CreateCommittedResource(
				&properties,						// ヒープの設定
				D3D12_HEAP_FLAG_NONE,				// ヒープオプション(設定なし)
				&resource_desc,						// リソースの設定
				D3D12_RESOURCE_STATE_GENERIC_READ,	// リソースの状態
				nullptr,							// クリアカラーのデフォルト値
				IID_PPV_ARGS(&buffer))))
			{
				throw "頂点バッファの作成に失敗しました。";
			}

			// 頂点バッファ(リソース)へのポインタ
			unsigned char* pData;
			if (FAILED(buffer->Map(
				0,									// サブリソースのインデックス番号
				nullptr,							// CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
				reinterpret_cast<void**>(&pData))))	// リソースデータへのポインタ
			{
				throw "頂点バッファ用リソースへのポインタの取得に失敗しました。";
			}

			// 頂点バッファ(リソース)へ頂点データをコピー
			std::memcpy(pData, vertices, size);

			// 頂点バッファ(リソース)へのポインタを無効にする
			buffer->Unmap(
				0,									// サブリソースインデックス番号
				nullptr								// マップ解除するメモリの範囲、CPUが変更した可能性のある領域(nullptrは全領域)
			);

			// 頂点バッファのビューを初期化する
			view.BufferLocation = buffer->GetGPUVirtualAddress();	// バッファのアドレス
			view.SizeInBytes = size;								// バッファ(全頂点合計)のサイズ(バイト単位)
			view.StrideInBytes = stride;							// 1頂点のサイズ(バイト単位)
		}

		/**
		 *	@brief	ビューを取得する
		 *	@return	ビューへの参照
		 */
		const D3D12_VERTEX_BUFFER_VIEW& VertexBuffer::GetView() const
		{
			return view;
		}
	};
};
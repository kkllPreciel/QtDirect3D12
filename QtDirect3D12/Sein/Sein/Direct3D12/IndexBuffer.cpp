/**
 *	@file		IndexBuffer.cpp
 *	@brief		頂点インデックスバッファに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/05/10
 *	@version	1.0
 */

// include
#include <cstring>
#include "Buffer.h"
#include "IndexBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		IndexBuffer::IndexBuffer() : buffer(new Buffer)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		IndexBuffer::~IndexBuffer()
		{
			buffer.reset(nullptr);
		}

		/**
		 *	@brief	頂点インデックスバッファの生成を行う
		 *	@param	device:Direct3D12のデバイス
		 *	@param	size:頂点インデックスバッファのサイズ(全頂点インデックス合計サイズ)
		 *	@param	indices:頂点インデックスデータ
		 *	@param	format:データフォーマット
		 */
		void IndexBuffer::Create(ID3D12Device* const device, const unsigned int size, const void* const indices, DXGI_FORMAT format)
		{
			// ヒープの設定
			D3D12_HEAP_PROPERTIES properties;
			properties.Type = D3D12_HEAP_TYPE_UPLOAD;						// ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
			properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// CPUページプロパティ(不明に設定)
			properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;	// ヒープのメモリプール(不明に設定)
			properties.CreationNodeMask = 1;								// 恐らくヒープが生成されるアダプター(GPU)の番号
			properties.VisibleNodeMask = 1;									// 恐らくヒープが表示されるアダプター(GPU)の番号

			try
			{
				buffer->Create(device, properties, size, D3D12_RESOURCE_FLAG_NONE);
			}
			catch (const char*)
			{
				throw "インデックスバッファ用リソースの作成に失敗しました。";
			}

			// 頂点インデックスバッファ(リソース)へのポインタ
			unsigned char* pData;
			if (FAILED(buffer->Get().Map(
				0,									// サブリソースのインデックス番号
				nullptr,							// CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
				reinterpret_cast<void**>(&pData))))	// リソースデータへのポインタ
			{
				throw "インデックスバッファ用リソースへのポインタの取得に失敗しました。";
			}

			// 頂点インデックスバッファ(リソース)へ頂点インデックスデータをコピー
			std::memcpy(pData, indices, size);

			// 頂点インデックスバッファ(リソース)へのポインタを無効にする
			buffer->Get().Unmap(
				0,			// サブリソースインデックス番号
				nullptr		// マップ解除するメモリの範囲、CPUが変更した可能性のある領域(nullptrは全領域)
			);

			// 頂点インデックスバッファのビューを初期化する
			view.BufferLocation = buffer->Get().GetGPUVirtualAddress();	// バッファのアドレス
			view.SizeInBytes = size;								// バッファ(全頂点インデックス合計)のサイズ(バイト単位)
			view.Format = format;									// バッファのフォーマット
		}

		/**
		 *	@brief	ビューを取得する
		 *	@return	ビューへの参照
		 */
		const D3D12_INDEX_BUFFER_VIEW& IndexBuffer::GetView() const
		{
			return view;
		}
	};
};
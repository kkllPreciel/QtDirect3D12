/**
 *	@file		VertexBuffer.cpp
 *	@brief		頂点バッファに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/05/09
 *	@version	1.0
 */

// include
#include "Buffer.h"
#include "VertexBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		VertexBuffer::VertexBuffer() : buffer(new Buffer())
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		VertexBuffer::~VertexBuffer()
		{
			buffer.reset(nullptr);
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

			try
			{
				buffer->Create(device, properties, size, D3D12_RESOURCE_FLAG_NONE);
			}
			catch (const char*)
			{
				throw "頂点バッファ用リソースの作成に失敗しました。";
			}

			// 頂点バッファ(リソース)へのポインタ
			unsigned char* pData;
			if (FAILED(buffer->Get().Map(
				0,									// サブリソースのインデックス番号
				nullptr,							// CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
				reinterpret_cast<void**>(&pData))))	// リソースデータへのポインタ
			{
				throw "頂点バッファ用リソースへのポインタの取得に失敗しました。";
			}

			// 頂点バッファ(リソース)へ頂点データをコピー
			std::memcpy(pData, vertices, size);

			// 頂点バッファ(リソース)へのポインタを無効にする
			buffer->Get().Unmap(
				0,									// サブリソースインデックス番号
				nullptr								// マップ解除するメモリの範囲、CPUが変更した可能性のある領域(nullptrは全領域)
			);

			// 頂点バッファのビューを初期化する
			view.BufferLocation = buffer->Get().GetGPUVirtualAddress();	// バッファのアドレス
			view.SizeInBytes = size;									// バッファ(全頂点合計)のサイズ(バイト単位)
			view.StrideInBytes = stride;								// 1頂点のサイズ(バイト単位)
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
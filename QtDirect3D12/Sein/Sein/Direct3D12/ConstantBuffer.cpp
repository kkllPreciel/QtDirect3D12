/**
 *	@file		ConstantBuffer.cpp
 *	@brief		定数バッファに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/07/22
 *	@version	1.0
 */

// include
#include "Buffer.h"
#include "ConstantBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		ConstantBuffer::ConstantBuffer() : buffer(nullptr), buffer_pointer(nullptr)
		{

		}
		
		/**
		 *	@brief	デストラクタ
		 */
		ConstantBuffer::~ConstantBuffer()
		{
			Release();
		}

		/**
		 *	@brief	定数バッファを生成する
		 *	@param	device:Direct3D12のデバイス
		 *	@param	heap:ディスクリプターヒープ
		 *	@param	size:定数バッファのサイズ
		 */
		void ConstantBuffer::Create(ID3D12Device* const device, ID3D12DescriptorHeap* const heap, const unsigned int size) noexcept(false)
		{
			Release();

			// ヒープの設定
			D3D12_HEAP_PROPERTIES properties;
			properties.Type = D3D12_HEAP_TYPE_UPLOAD;						// ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
			properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// CPUページプロパティ(不明に設定)
			properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;	// ヒープのメモリプール(不明に設定)
			properties.CreationNodeMask = 1;								// 恐らくヒープが生成されるアダプター(GPU)の番号
			properties.VisibleNodeMask = 1;									// 恐らくヒープが表示されるアダプター(GPU)の番号

			try
			{
				buffer.reset(new Buffer());
				buffer->Create(device, properties, size, D3D12_RESOURCE_FLAG_NONE);
			}
			catch (const char*)
			{
				throw "定数バッファ用リソースの作成に失敗しました。";
			}

			// 定数バッファビューの設定
			D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
			constantBufferViewDesc.BufferLocation = buffer->Get().GetGPUVirtualAddress();		// バッファのアドレス
			constantBufferViewDesc.SizeInBytes = (size + 255) & ~255;							// 定数バッファは256バイトでアラインメントされていなければならない

			// 定数バッファビュー用のディスクリプターを生成
			// ディスクリプターヒープの領域に作成される
			device->CreateConstantBufferView(&constantBufferViewDesc, heap->GetCPUDescriptorHandleForHeapStart());

			// マップ。Releaseが呼ばれるまでアンマップしない
			if (FAILED(buffer->Get().Map(
				0,												// サブリソースのインデックス番号
				nullptr,										// CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
				reinterpret_cast<void**>(&buffer_pointer))))	// リソースデータへのポインタ
			{
				throw "定数バッファ用リソースへのポインタの取得に失敗しました。";
			}

			// 初期化
			std::memset(buffer_pointer, 0, size);
		}
		
		/**
		 *	@brief	定数バッファを開放する
		 */
		void ConstantBuffer::Release() noexcept
		{
			if (buffer && buffer_pointer)
			{
				buffer->Get().Unmap(0, nullptr);
			}

			if (buffer)
			{
				buffer.reset(nullptr);
			}

			buffer_pointer = nullptr;
		}
		
		/**
		 *	@brief	データのマップを行う
		 *	@param	data:マップするデータ
		 *	@param	size:マップするデータのサイズ
		 */
		void ConstantBuffer::Map(const void* const data, const unsigned int size) noexcept(false)
		{
			if (nullptr == buffer_pointer)
			{
				throw "定数バッファ用リソースへのポインタが存在しません。";
			}

			std::memcpy(buffer_pointer, data, size);
		}
	};
};
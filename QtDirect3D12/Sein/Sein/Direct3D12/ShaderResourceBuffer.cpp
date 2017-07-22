/**
 *	@file		ShaderResourceBuffer.cpp
 *	@brief		シェーダーリソースバッファに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/07/22
 *	@version	1.0
 */

// include
#include "Buffer.h"
#include "ShaderResourceBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		ShaderResourceBuffer::ShaderResourceBuffer() : buffer(nullptr), buffer_pointer(nullptr)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		ShaderResourceBuffer::~ShaderResourceBuffer()
		{
			Release();
		}

		/**
		 *	@brief	シェーダーリソースバッファを生成する
		 *	@param	device:Direct3D12のデバイス
		 *	@param	descriptor_handle:ディスクリプターハンドル
		 *	@param	num:リソース内の要素数
		 *	@param	size:リソース内の1要素のサイズ
		 */
		void ShaderResourceBuffer::Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE* const descriptor_handle, const unsigned int num, const unsigned int size) noexcept(false)
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
				buffer->Create(device, properties, size * num, D3D12_RESOURCE_FLAG_NONE);
			}
			catch (const char*)
			{
				throw "シェーダーリソースバッファ用リソースの作成に失敗しました。";
			}

			// シェーダーリソースビューの設定
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Buffer.NumElements = num;
			srvDesc.Buffer.StructureByteStride = size;
			srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

			// シェーダーリソースビュー用のディスクリプターを生成
			// ディスクリプターヒープの領域に作成される
			device->CreateShaderResourceView(&(buffer->Get()), &srvDesc, *descriptor_handle);

			// マップ。Releaseが呼ばれるまでアンマップしない
			if (FAILED(buffer->Get().Map(
				0,												// サブリソースのインデックス番号
				nullptr,										// CPUからアクセスするメモリの範囲(nullptrは全領域にアクセスする)
				reinterpret_cast<void**>(&buffer_pointer))))	// リソースデータへのポインタ
			{
				throw "シェーダーリソースバッファ用リソースへのポインタの取得に失敗しました。";
			}

			// 初期化
			std::memset(buffer_pointer, 0, size * num);
		}

		/**
		 *	@brief	シェーダーリソースバッファを開放する
		 */
		void ShaderResourceBuffer::Release() noexcept
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
		void ShaderResourceBuffer::Map(const void* const data, const unsigned int size) noexcept(false)
		{
			if (nullptr == buffer_pointer)
			{
				throw "シェーダーリソースバッファ用リソースへのポインタが存在しません。";
			}

			std::memcpy(buffer_pointer, data, size);
		}
	};
};
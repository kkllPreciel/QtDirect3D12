/**
 *	@file		ShaderResourceBuffer.h
 *	@brief		シェーダーリソース用バッファに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/07/22
 *	@version	1.0
 */

#pragma once

// include
#include <d3d12.h>
#include <memory>

namespace Sein
{
	namespace Direct3D12
	{
		class Buffer;

		/**
		 *	@brief	シェーダーリソースバッファクラス
		 */
		class ShaderResourceBuffer
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			ShaderResourceBuffer();

			/**
			 *	@brief	デストラクタ
			 */
			~ShaderResourceBuffer();

			/**
			 *	@brief	シェーダーリソースバッファを生成する
			 *	@param	device:Direct3D12のデバイス
			 *	@param	descriptor_handle:ディスクリプターハンドル
			 *	@param	num:リソース内の要素数
			 *	@param	size:リソース内の1要素のサイズ			 
			 */
			void Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE* const descriptor_handle, const unsigned int num, const unsigned int size) noexcept(false);

			/**
			 *	@brief	シェーダーリソースバッファを開放する
			 */
			void Release() noexcept;

			/**
			 *	@brief	データのマップを行う
			 *	@param	data:マップするデータ
			 *	@param	size:マップするデータのサイズ
			 */
			void Map(const void* const data, const unsigned int size) noexcept(false);

		private:
			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			ShaderResourceBuffer(const ShaderResourceBuffer& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			ShaderResourceBuffer& operator = (const ShaderResourceBuffer& other) = delete;

		private:
			std::unique_ptr<Buffer> buffer; ///< バッファ(リソース)
			void* buffer_pointer;			///< バッファへのポインタ
		};
	};
};
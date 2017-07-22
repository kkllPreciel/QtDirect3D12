/**
 *	@file		ConstantBuffer.h
 *	@brief		定数バッファに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/07/21
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
		 *	@brief	定数バッファクラス
		 */
		class ConstantBuffer
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			ConstantBuffer();

			/**
			 *	@brief	デストラクタ
			 */
			~ConstantBuffer();

			/**
			 *	@brief	定数バッファを生成する
			 *	@param	device:Direct3D12のデバイス
			 *	@param	descriptor_handle:ディスクリプターハンドル
			 *	@param	size:定数バッファのサイズ
			 */
			void Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, const unsigned int size) noexcept(false);

			/**
			 *	@brief	定数バッファを開放する
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
			ConstantBuffer(const ConstantBuffer& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			ConstantBuffer& operator = (const ConstantBuffer& other) = delete;

		private:
			std::unique_ptr<Buffer> buffer; ///< バッファ(リソース)
			void* buffer_pointer;			///< バッファへのポインタ
		};
	};
};
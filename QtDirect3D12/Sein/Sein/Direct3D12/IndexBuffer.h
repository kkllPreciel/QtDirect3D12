/**
 *	@file		IndexBuffer.h
 *	@brief		頂点インデックスバッファに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/05/08
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
		 *	@brief	頂点インデックスバッファクラス
		 */
		class IndexBuffer
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			IndexBuffer();

			/**
			 *	@brief	デストラクタ
			 */
			~IndexBuffer();

			/**
			 *	@brief	頂点インデックスバッファの生成を行う
			 *	@param	device:Direct3D12のデバイス
			 *	@param	size:頂点インデックスバッファのサイズ(全頂点インデックス合計サイズ)
			 *	@param	indices:頂点インデックスデータ
			 *	@param	format:データフォーマット
			 */
			void Create(ID3D12Device* const device, const unsigned int size, const void* const indices, DXGI_FORMAT format);

			/**
			 *	@brief	ビューを取得する
			 *	@return	ビューへの参照
			 */
			const D3D12_INDEX_BUFFER_VIEW& GetView() const;

		private:
			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			IndexBuffer(const IndexBuffer& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			IndexBuffer& operator = (const IndexBuffer& other) = delete;

		private:
			std::unique_ptr<Buffer>		buffer; ///< バッファ(リソース)
			D3D12_INDEX_BUFFER_VIEW		view;	///< 頂点インデックスバッファのビュー
		};
	};
};
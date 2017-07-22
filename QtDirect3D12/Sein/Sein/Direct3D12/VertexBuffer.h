/**
 *	@file		VertexBuffer.h
 *	@brief		頂点バッファに関するヘッダファイル
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
		 *	@brief	頂点バッファクラス
		 */
		class VertexBuffer
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			VertexBuffer();

			/**
			 *	@brief	デストラクタ
			 */
			~VertexBuffer();

			/**
			 *	@brief	頂点バッファの生成を行う
			 *	@param	device:Direct3D12のデバイス
			 *	@param	size:頂点バッファのサイズ(全頂点合計サイズ)
			 *	@param	stride:1頂点のサイズ
			 *	@param	vertices:頂点データ
			 */
			void Create(ID3D12Device* const device, const unsigned int size, const unsigned short stride, const void* const vertices);

			/**
			 *	@brief	ビューを取得する
			 *	@return	ビューへの参照
			 */
			const D3D12_VERTEX_BUFFER_VIEW& GetView() const;			

		private:
			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			VertexBuffer(const VertexBuffer& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			VertexBuffer& operator = (const VertexBuffer& other) = delete;

		private:
			std::unique_ptr<Buffer>		buffer; ///< バッファ(リソース)
			D3D12_VERTEX_BUFFER_VIEW	view;	///< 頂点バッファのビュー
		};
	};
};
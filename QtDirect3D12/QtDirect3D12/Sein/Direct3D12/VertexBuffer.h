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

namespace Sein
{
	namespace Direct3D12
	{
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
			 */
			void Create(const ID3D12Device& device);

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
			ID3D12Resource*				buffer;	///< 頂点バッファ
			D3D12_VERTEX_BUFFER_VIEW	view;	///< 頂点バッファのビュー
		};
	};
};
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

namespace Sein
{
	namespace Direct3D12
	{
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
			 */
			void Create(const ID3D12Device& device);

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
			ID3D12Resource*				buffer;	///< 頂点インデックスバッファ
			D3D12_INDEX_BUFFER_VIEW		view;	///< 頂点インデックスバッファのビュー
		};
	};
};
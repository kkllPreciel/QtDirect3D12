/**
 *	@file		Mesh.h
 *	@brief		メッシュに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/07/16
 *	@version	1.0
 */

#pragma once

// include
#include <memory>
#include <d3d12.h>

namespace Sein
{
	namespace Direct3D12
	{
		class VertexBuffer;
		class IndexBuffer;

		/**
		 *	@brief	メッシュクラス
		 */
		class Mesh
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			Mesh();

			/**
			 *	@brief	デストラクタ
			 */
			~Mesh();

			/**
			 *	@brief	頂点バッファを取得する
			 *	@return	頂点バッファへの参照
			 */
			const VertexBuffer& GetVertexBuffer() const;

			/**
			 *	@brief	インデックスバッファを取得する
			 *	@return	インデックスバッファへの参照
			 */
			const IndexBuffer& GetIndexBuffer() const;

		private:
			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			Mesh(const Mesh& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			Mesh& operator = (const Mesh& other) = delete;

		private:
			std::unique_ptr<VertexBuffer>	vertexBuffer;	///< 頂点バッファ
			std::unique_ptr<IndexBuffer>	indexBuffer;	///< 頂点インデックスバッファ
		};
	};
};
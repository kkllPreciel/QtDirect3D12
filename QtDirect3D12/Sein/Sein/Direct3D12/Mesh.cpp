/**
 *	@file		Mesh.cpp
 *	@brief		メッシュに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/07/16
 *	@version	1.0
 */

// include
#include <cstring>
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		Mesh::Mesh() : vertexBuffer(nullptr), indexBuffer(nullptr)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		Mesh::~Mesh()
		{

		}

		/**
		 *	@brief	頂点バッファを取得する
		 *	@return	頂点バッファへの参照
		 */
		const VertexBuffer& Mesh::GetVertexBuffer() const
		{
			return *vertexBuffer;
		}

		/**
		 *	@brief	インデックスバッファを取得する
		 *	@return	インデックスバッファへの参照
		 */
		const IndexBuffer& Mesh::GetIndexBuffer() const
		{
			return *indexBuffer;
		}
	};
};
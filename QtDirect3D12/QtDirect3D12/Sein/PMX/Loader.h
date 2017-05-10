/**
 *	@file		Loeader.h
 *	@brief		Pmx読み込みに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/04/30
 *	@version	1.0
 */

#pragma once

// include
#include <string>

namespace Sein
{
	namespace Pmx
	{
		struct Header;
		struct Vertex;

		/**
		 *	@brief	Pmx読み込み用クラス
		 */
		class Loader
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			Loader();

			/**
			 *	@brief	デストラクタ
			 */
			~Loader();

			/**
			 *	@brief	読み込みを行う
			 *	@param	filePath:読み込みを行うPmxファイルのパス
			 */
			void Load(std::string filePath);

			/**
			 *	@brief	頂点数を取得する
			 *	@return	頂点数
			 */
			unsigned int GetVertexCount() const;

			/**
			 *	@brief	頂点データを取得する
			 *	@return	頂点データへのポインタ
			 */
			const void* const GetVertices() const;

			/**
			 *	@brief	頂点のサイズを取得する
			 *	@return	頂点のサイズ
			 */
			unsigned int GetVertexSize() const;

			/**
			 *	@brief	頂点インデックス数を取得する
			 *	@return	頂点インデックス数
			 */
			unsigned int GetIndexCount() const;

			/**
			 *	@brief	頂点インデックスデータを取得する
			 *	@return	頂点インデックスデータへのポインタ
			 */
			const void* const GetIndices() const;

			/**
			 *	@brief	頂点インデックスのサイズを取得する
			 *	@return	頂点インデックスのサイズ
			 */
			const unsigned int GetIndexSize() const;

		private:
			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			Loader(const Loader& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			Loader& operator = (const Loader& other) = delete;

		private:
			char*			buffer;			///< バッファ
			unsigned int	size;			///< ファイルサイズ
			Header*			header;			///< ヘッダ
			unsigned int	vertexCount;	///< 頂点数
			Vertex*			vertices;		///< 頂点データリスト
			unsigned int	indexCount;		///< 頂点インデックス数
			unsigned int*	indices;		///< 頂点インデックスデータリスト
			unsigned int	polygonCount;	///< ポリゴン数
		};
	};
};
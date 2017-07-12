/**
 *	@file		InstanceBuffer.h
 *	@brief		インスタンシング用バッファに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/07/12
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
		 *	@brief	インスタンス用バッファクラス
		 */
		class InstanceBuffer
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			InstanceBuffer();

			/**
			 *	@brief	デストラクタ
			 */
			~InstanceBuffer();

			/**
			 *	@brief	インスタンスバッファを生成する
			 *	@param	device:Direct3D12のデバイス
			 */
			void Create(ID3D12Device* const device);

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
			InstanceBuffer(const InstanceBuffer& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			InstanceBuffer& operator = (const InstanceBuffer& other) = delete;

		private:
			ID3D12Resource*				buffer;	///< インスタンスバッファ
			D3D12_VERTEX_BUFFER_VIEW	view;	///< インスタンスバッファのビュー
		};
	};
};
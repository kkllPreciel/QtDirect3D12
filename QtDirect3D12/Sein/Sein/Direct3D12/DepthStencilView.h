/**
 *	@file		DepthStencilView.h
 *	@brief		深度ステンシルビューに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/05/26
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
		 *	@brief	深度ステンシルビュークラス
		 */
		class DepthStencilView
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			DepthStencilView();

			/**
			 *	@brief	デストラクタ
			 */
			~DepthStencilView();

			/**
			 *	@brief	深度ステンシルビューの生成を行う
			 *	@param	device:Direct3D12のデバイス
			 *	@param	width:横幅
			 *	@param	height:縦幅
			 */
			void Create(ID3D12Device* const device, unsigned int width, unsigned int height);

			/**
			 *	@brief	ディスクリプターハンドルを取得する
			 *	@return	ディスクリプターハンドル
			 */
			D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const;

			/**
			 *	@brief	リソースを開放する
			 */
			void Release();

		private:
			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			DepthStencilView(const DepthStencilView& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			DepthStencilView& operator = (const DepthStencilView& other) = delete;

		private:
			ID3D12Resource*			depthStencil;			///< リソース
			ID3D12DescriptorHeap*	depthStencilViewHeap;	///< ディスクリプターヒープ
		};
	};
};
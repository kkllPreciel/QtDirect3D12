#pragma once
/**
 *	@file		Fence.h
 *	@brief		フェンスに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/07/05
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
		 *	@brief	フェンスクラス
		 */
		class Fence
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			Fence();

			/**
			 *	@brief	デストラクタ
			 */
			~Fence();

			/**
			 *	@brief	リソースを開放する
			 */
			void Release();

		private:
			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			Fence(const Fence& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			Fence& operator = (const Fence& other) = delete;

			ID3D12Fence*	fence;			///< フェンス
			unsigned int	index;			///< 現在の番号
			HANDLE			eventHandle;	///< イベントハンドル
		};
	};
};
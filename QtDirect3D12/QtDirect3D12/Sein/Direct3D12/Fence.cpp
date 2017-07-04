/**
 *	@file		Fence.cpp
 *	@brief		フェンスに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/07/05
 *	@version	1.0
 */

#include "Fence.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		Fence::Fence() : fence(nullptr), index(0), eventHandle(nullptr)
		{

		}
		
		/**
		 *	@brief	デストラクタ
		 */
		Fence::~Fence()
		{
			Release();
		}
		
		/**
		 *	@brief	リソースを開放する
		 */
		void Fence::Release()
		{
			if (nullptr != eventHandle)
			{
				CloseHandle(eventHandle);
			}

			if (nullptr != fence)
			{
				fence->Release();
			}
		}
	};
};
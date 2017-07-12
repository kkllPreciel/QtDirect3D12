/**
 *	@file		InstanceBuffer.cpp
 *	@brief		インスタンシング用バッファに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/07/12
 *	@version	1.0
 */

// include
#include <cstring>
#include "InstanceBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		InstanceBuffer::InstanceBuffer() : buffer(nullptr)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		InstanceBuffer::~InstanceBuffer()
		{
			if (nullptr != buffer)
			{
				buffer->Release();
				buffer = nullptr;
			}
		}

		/**
		 *	@brief	インスタンスバッファを生成する
		 *	@param	device:Direct3D12のデバイス
		 */
		void InstanceBuffer::Create(ID3D12Device* const device)
		{

		}

		/**
		 *	@brief	ビューを取得する
		 *	@return	ビューへの参照
		 */
		const D3D12_VERTEX_BUFFER_VIEW& InstanceBuffer::GetView() const
		{
			return view;
		}
	};
};
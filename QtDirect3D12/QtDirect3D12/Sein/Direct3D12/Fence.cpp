/**
 *	@file		Fence.cpp
 *	@brief		�t�F���X�Ɋւ��鏈�����s���v���O�����\�[�X
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
		 *	@brief	�R���X�g���N�^
		 */
		Fence::Fence() : fence(nullptr), index(0), eventHandle(nullptr)
		{

		}
		
		/**
		 *	@brief	�f�X�g���N�^
		 */
		Fence::~Fence()
		{
			Release();
		}
		
		/**
		 *	@brief	���\�[�X���J������
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
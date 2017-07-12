/**
 *	@file		InstanceBuffer.cpp
 *	@brief		�C���X�^���V���O�p�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
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
		 *	@brief	�R���X�g���N�^
		 */
		InstanceBuffer::InstanceBuffer() : buffer(nullptr)
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
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
		 *	@brief	�C���X�^���X�o�b�t�@�𐶐�����
		 *	@param	device:Direct3D12�̃f�o�C�X
		 */
		void InstanceBuffer::Create(ID3D12Device* const device)
		{

		}

		/**
		 *	@brief	�r���[���擾����
		 *	@return	�r���[�ւ̎Q��
		 */
		const D3D12_VERTEX_BUFFER_VIEW& InstanceBuffer::GetView() const
		{
			return view;
		}
	};
};
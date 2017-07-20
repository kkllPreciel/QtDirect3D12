/**
 *	@file		VertexBuffer.cpp
 *	@brief		���_�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/05/09
 *	@version	1.0
 */

// include
#include "Buffer.h"
#include "VertexBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�R���X�g���N�^
		 */
		VertexBuffer::VertexBuffer() : buffer(new Buffer())
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
		 */
		VertexBuffer::~VertexBuffer()
		{
			buffer.reset(nullptr);
		}

		/**
		 *	@brief	���_�o�b�t�@�̐������s��
		 *	@param	device:Direct3D12�̃f�o�C�X
		 *	@param	size:���_�o�b�t�@�̃T�C�Y(�S���_���v�T�C�Y)
		 *	@param	stride:1���_�̃T�C�Y
		 *	@param	vertices:���_�f�[�^
		 */
		void VertexBuffer::Create(ID3D12Device* const device, const unsigned int size, const unsigned short stride, const void* const vertices)
		{
			// �q�[�v�̐ݒ�
			D3D12_HEAP_PROPERTIES properties;
			properties.Type = D3D12_HEAP_TYPE_UPLOAD;						// �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
			properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
			properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;	// �q�[�v�̃������v�[��(�s���ɐݒ�)
			properties.CreationNodeMask = 1;								// ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
			properties.VisibleNodeMask = 1;									// ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

			try
			{
				buffer->Create(device, properties, size, D3D12_RESOURCE_FLAG_NONE);
			}
			catch (const char*)
			{
				throw "���_�o�b�t�@�p���\�[�X�̍쐬�Ɏ��s���܂����B";
			}

			// ���_�o�b�t�@(���\�[�X)�ւ̃|�C���^
			unsigned char* pData;
			if (FAILED(buffer->Get().Map(
				0,									// �T�u���\�[�X�̃C���f�b�N�X�ԍ�
				nullptr,							// CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
				reinterpret_cast<void**>(&pData))))	// ���\�[�X�f�[�^�ւ̃|�C���^
			{
				throw "���_�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B";
			}

			// ���_�o�b�t�@(���\�[�X)�֒��_�f�[�^���R�s�[
			std::memcpy(pData, vertices, size);

			// ���_�o�b�t�@(���\�[�X)�ւ̃|�C���^�𖳌��ɂ���
			buffer->Get().Unmap(
				0,									// �T�u���\�[�X�C���f�b�N�X�ԍ�
				nullptr								// �}�b�v�������郁�����͈̔́ACPU���ύX�����\���̂���̈�(nullptr�͑S�̈�)
			);

			// ���_�o�b�t�@�̃r���[������������
			view.BufferLocation = buffer->Get().GetGPUVirtualAddress();	// �o�b�t�@�̃A�h���X
			view.SizeInBytes = size;									// �o�b�t�@(�S���_���v)�̃T�C�Y(�o�C�g�P��)
			view.StrideInBytes = stride;								// 1���_�̃T�C�Y(�o�C�g�P��)
		}

		/**
		 *	@brief	�r���[���擾����
		 *	@return	�r���[�ւ̎Q��
		 */
		const D3D12_VERTEX_BUFFER_VIEW& VertexBuffer::GetView() const
		{
			return view;
		}
	};
};
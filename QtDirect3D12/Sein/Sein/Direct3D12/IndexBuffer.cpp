/**
 *	@file		IndexBuffer.cpp
 *	@brief		���_�C���f�b�N�X�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/05/10
 *	@version	1.0
 */

// include
#include <cstring>
#include "Buffer.h"
#include "IndexBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�R���X�g���N�^
		 */
		IndexBuffer::IndexBuffer() : buffer(new Buffer)
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
		 */
		IndexBuffer::~IndexBuffer()
		{
			buffer.reset(nullptr);
		}

		/**
		 *	@brief	���_�C���f�b�N�X�o�b�t�@�̐������s��
		 *	@param	device:Direct3D12�̃f�o�C�X
		 *	@param	size:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(�S���_�C���f�b�N�X���v�T�C�Y)
		 *	@param	indices:���_�C���f�b�N�X�f�[�^
		 *	@param	format:�f�[�^�t�H�[�}�b�g
		 */
		void IndexBuffer::Create(ID3D12Device* const device, const unsigned int size, const void* const indices, DXGI_FORMAT format)
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
				throw "�C���f�b�N�X�o�b�t�@�p���\�[�X�̍쐬�Ɏ��s���܂����B";
			}

			// ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�ւ̃|�C���^
			unsigned char* pData;
			if (FAILED(buffer->Get().Map(
				0,									// �T�u���\�[�X�̃C���f�b�N�X�ԍ�
				nullptr,							// CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
				reinterpret_cast<void**>(&pData))))	// ���\�[�X�f�[�^�ւ̃|�C���^
			{
				throw "�C���f�b�N�X�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B";
			}

			// ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�֒��_�C���f�b�N�X�f�[�^���R�s�[
			std::memcpy(pData, indices, size);

			// ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�ւ̃|�C���^�𖳌��ɂ���
			buffer->Get().Unmap(
				0,			// �T�u���\�[�X�C���f�b�N�X�ԍ�
				nullptr		// �}�b�v�������郁�����͈̔́ACPU���ύX�����\���̂���̈�(nullptr�͑S�̈�)
			);

			// ���_�C���f�b�N�X�o�b�t�@�̃r���[������������
			view.BufferLocation = buffer->Get().GetGPUVirtualAddress();	// �o�b�t�@�̃A�h���X
			view.SizeInBytes = size;								// �o�b�t�@(�S���_�C���f�b�N�X���v)�̃T�C�Y(�o�C�g�P��)
			view.Format = format;									// �o�b�t�@�̃t�H�[�}�b�g
		}

		/**
		 *	@brief	�r���[���擾����
		 *	@return	�r���[�ւ̎Q��
		 */
		const D3D12_INDEX_BUFFER_VIEW& IndexBuffer::GetView() const
		{
			return view;
		}
	};
};
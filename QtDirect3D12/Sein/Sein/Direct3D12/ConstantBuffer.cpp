/**
 *	@file		ConstantBuffer.cpp
 *	@brief		�萔�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/07/22
 *	@version	1.0
 */

// include
#include "Buffer.h"
#include "ConstantBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�R���X�g���N�^
		 */
		ConstantBuffer::ConstantBuffer() : buffer(nullptr), buffer_pointer(nullptr)
		{

		}
		
		/**
		 *	@brief	�f�X�g���N�^
		 */
		ConstantBuffer::~ConstantBuffer()
		{
			Release();
		}

		/**
		 *	@brief	�萔�o�b�t�@�𐶐�����
		 *	@param	device:Direct3D12�̃f�o�C�X
		 *	@param	heap:�f�B�X�N���v�^�[�q�[�v
		 *	@param	size:�萔�o�b�t�@�̃T�C�Y
		 */
		void ConstantBuffer::Create(ID3D12Device* const device, ID3D12DescriptorHeap* const heap, const unsigned int size) noexcept(false)
		{
			Release();

			// �q�[�v�̐ݒ�
			D3D12_HEAP_PROPERTIES properties;
			properties.Type = D3D12_HEAP_TYPE_UPLOAD;						// �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
			properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
			properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;	// �q�[�v�̃������v�[��(�s���ɐݒ�)
			properties.CreationNodeMask = 1;								// ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
			properties.VisibleNodeMask = 1;									// ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

			try
			{
				buffer.reset(new Buffer());
				buffer->Create(device, properties, size, D3D12_RESOURCE_FLAG_NONE);
			}
			catch (const char*)
			{
				throw "�萔�o�b�t�@�p���\�[�X�̍쐬�Ɏ��s���܂����B";
			}

			// �萔�o�b�t�@�r���[�̐ݒ�
			D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
			constantBufferViewDesc.BufferLocation = buffer->Get().GetGPUVirtualAddress();		// �o�b�t�@�̃A�h���X
			constantBufferViewDesc.SizeInBytes = (size + 255) & ~255;							// �萔�o�b�t�@��256�o�C�g�ŃA���C�������g����Ă��Ȃ���΂Ȃ�Ȃ�

			// �萔�o�b�t�@�r���[�p�̃f�B�X�N���v�^�[�𐶐�
			// �f�B�X�N���v�^�[�q�[�v�̗̈�ɍ쐬�����
			device->CreateConstantBufferView(&constantBufferViewDesc, heap->GetCPUDescriptorHandleForHeapStart());

			// �}�b�v�BRelease���Ă΂��܂ŃA���}�b�v���Ȃ�
			if (FAILED(buffer->Get().Map(
				0,												// �T�u���\�[�X�̃C���f�b�N�X�ԍ�
				nullptr,										// CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
				reinterpret_cast<void**>(&buffer_pointer))))	// ���\�[�X�f�[�^�ւ̃|�C���^
			{
				throw "�萔�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B";
			}

			// ������
			std::memset(buffer_pointer, 0, size);
		}
		
		/**
		 *	@brief	�萔�o�b�t�@���J������
		 */
		void ConstantBuffer::Release() noexcept
		{
			if (buffer && buffer_pointer)
			{
				buffer->Get().Unmap(0, nullptr);
			}

			if (buffer)
			{
				buffer.reset(nullptr);
			}

			buffer_pointer = nullptr;
		}
		
		/**
		 *	@brief	�f�[�^�̃}�b�v���s��
		 *	@param	data:�}�b�v����f�[�^
		 *	@param	size:�}�b�v����f�[�^�̃T�C�Y
		 */
		void ConstantBuffer::Map(const void* const data, const unsigned int size) noexcept(false)
		{
			if (nullptr == buffer_pointer)
			{
				throw "�萔�o�b�t�@�p���\�[�X�ւ̃|�C���^�����݂��܂���B";
			}

			std::memcpy(buffer_pointer, data, size);
		}
	};
};
/**
 *	@file		ShaderResourceBuffer.cpp
 *	@brief		�V�F�[�_�[���\�[�X�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/07/22
 *	@version	1.0
 */

// include
#include "Buffer.h"
#include "ShaderResourceBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�R���X�g���N�^
		 */
		ShaderResourceBuffer::ShaderResourceBuffer() : buffer(nullptr), buffer_pointer(nullptr)
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
		 */
		ShaderResourceBuffer::~ShaderResourceBuffer()
		{
			Release();
		}

		/**
		 *	@brief	�V�F�[�_�[���\�[�X�o�b�t�@�𐶐�����
		 *	@param	device:Direct3D12�̃f�o�C�X
		 *	@param	descriptor_handle:�f�B�X�N���v�^�[�n���h��
		 *	@param	num:���\�[�X���̗v�f��
		 *	@param	size:���\�[�X����1�v�f�̃T�C�Y
		 */
		void ShaderResourceBuffer::Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE* const descriptor_handle, const unsigned int num, const unsigned int size) noexcept(false)
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
				buffer->Create(device, properties, size * num, D3D12_RESOURCE_FLAG_NONE);
			}
			catch (const char*)
			{
				throw "�V�F�[�_�[���\�[�X�o�b�t�@�p���\�[�X�̍쐬�Ɏ��s���܂����B";
			}

			// �V�F�[�_�[���\�[�X�r���[�̐ݒ�
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Buffer.NumElements = num;
			srvDesc.Buffer.StructureByteStride = size;
			srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

			// �V�F�[�_�[���\�[�X�r���[�p�̃f�B�X�N���v�^�[�𐶐�
			// �f�B�X�N���v�^�[�q�[�v�̗̈�ɍ쐬�����
			device->CreateShaderResourceView(&(buffer->Get()), &srvDesc, *descriptor_handle);

			// �}�b�v�BRelease���Ă΂��܂ŃA���}�b�v���Ȃ�
			if (FAILED(buffer->Get().Map(
				0,												// �T�u���\�[�X�̃C���f�b�N�X�ԍ�
				nullptr,										// CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
				reinterpret_cast<void**>(&buffer_pointer))))	// ���\�[�X�f�[�^�ւ̃|�C���^
			{
				throw "�V�F�[�_�[���\�[�X�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B";
			}

			// ������
			std::memset(buffer_pointer, 0, size * num);
		}

		/**
		 *	@brief	�V�F�[�_�[���\�[�X�o�b�t�@���J������
		 */
		void ShaderResourceBuffer::Release() noexcept
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
		void ShaderResourceBuffer::Map(const void* const data, const unsigned int size) noexcept(false)
		{
			if (nullptr == buffer_pointer)
			{
				throw "�V�F�[�_�[���\�[�X�o�b�t�@�p���\�[�X�ւ̃|�C���^�����݂��܂���B";
			}

			std::memcpy(buffer_pointer, data, size);
		}
	};
};
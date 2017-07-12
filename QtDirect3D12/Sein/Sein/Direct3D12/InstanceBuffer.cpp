/**
 *	@file		InstanceBuffer.cpp
 *	@brief		�C���X�^���V���O�p�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/07/12
 *	@version	1.0
 */

// include
#include <cstring>
#include <DirectXMath.h>
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
			// �q�[�v�̐ݒ�
			D3D12_HEAP_PROPERTIES properties;
			properties.Type = D3D12_HEAP_TYPE_UPLOAD;						// �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
			properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
			properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;	// �q�[�v�̃������v�[��(�s���ɐݒ�)
			properties.CreationNodeMask = 1;								// ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
			properties.VisibleNodeMask = 1;									// ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

																			// ���\�[�X�̐ݒ�
			D3D12_RESOURCE_DESC resource_desc;
			resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;		// ���\�[�X�̎��(����̓o�b�t�@)
			resource_desc.Alignment = 0;									// �A���C�������g
			resource_desc.Width = 4 * 16;									// ���\�[�X�̕�(����͒��_�C���f�b�N�X�o�b�t�@�̃T�C�Y)
			resource_desc.Height = 1;										// ���\�[�X�̍���(����͒��_�C���f�b�N�X�o�b�t�@���̕����m�ۂ��Ă���̂�1)
			resource_desc.DepthOrArraySize = 1;								// ���\�[�X�̐[��(�e�N�X�`�����Ɏg�p���镨�A�����1)
			resource_desc.MipLevels = 1;									// �~�b�v�}�b�v�̃��x��(�����1)
			resource_desc.Format = DXGI_FORMAT_UNKNOWN;						// ���\�[�X�f�[�^�t�H�[�}�b�g(R8G8B8A8��)(�����4�o�C�gfloat * 4)
			resource_desc.SampleDesc.Count = 1;								// �s�N�Z���P�ʂ̃}���`�T���v�����O��
			resource_desc.SampleDesc.Quality = 0;							// �}���`�T���v�����O�̕i�����x��
			resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;			// �e�N�X�`�����C�A�E�g�I�v�V����
			resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;					// ���\�[�X����I�v�V�����t���O(����͖���)

																			// ���\�[�X�̐���(�q�[�v�������ɐ��������)
			if (FAILED(device->CreateCommittedResource(
				&properties,						// �q�[�v�̐ݒ�
				D3D12_HEAP_FLAG_NONE,				// �q�[�v�I�v�V����(�ݒ�Ȃ�)
				&resource_desc,						// ���\�[�X�̐ݒ�
				D3D12_RESOURCE_STATE_GENERIC_READ,	// ���\�[�X�̏��
				nullptr,							// �N���A�J���[�̃f�t�H���g�l
				IID_PPV_ARGS(&buffer))))
			{
				throw "�C���X�^���X�p�o�b�t�@�̍쐬�Ɏ��s���܂����B";
			}

			// ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�ւ̃|�C���^
			unsigned char* pData;
			if (FAILED(buffer->Map(
				0,									// �T�u���\�[�X�̃C���f�b�N�X�ԍ�
				nullptr,							// CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
				reinterpret_cast<void**>(&pData))))	// ���\�[�X�f�[�^�ւ̃|�C���^
			{
				throw "���_�C���f�b�N�X�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B";
			}

			// ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�֒��_�C���f�b�N�X�f�[�^���R�s�[
			DirectX::XMFLOAT4X4 matrix;
			DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());
			std::memcpy(pData, &matrix, sizeof(matrix));

			// ���_�C���f�b�N�X�o�b�t�@(���\�[�X)�ւ̃|�C���^�𖳌��ɂ���
			buffer->Unmap(
				0,			// �T�u���\�[�X�C���f�b�N�X�ԍ�
				nullptr		// �}�b�v�������郁�����͈̔́ACPU���ύX�����\���̂���̈�(nullptr�͑S�̈�)
			);

			// ���_�C���f�b�N�X�o�b�t�@�̃r���[������������
			view.BufferLocation = buffer->GetGPUVirtualAddress();	// �o�b�t�@�̃A�h���X
			view.SizeInBytes = 4 * 16;								// �o�b�t�@(�S���_���v)�̃T�C�Y(�o�C�g�P��)
			view.StrideInBytes = 4 * 16;							// 1���_�̃T�C�Y(�o�C�g�P��)
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
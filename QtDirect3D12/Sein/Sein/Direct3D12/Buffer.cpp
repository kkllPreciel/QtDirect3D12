/**
 *	@file		Buffer.cpp
 *	@brief		�o�b�t�@�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/07/19
 *	@version	1.0
 */

// include
#include "Buffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�R���X�g���N�^
		 */
		Buffer::Buffer() : buffer(nullptr)
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
		 */
		Buffer::~Buffer()
		{
			Release();
		}

		/**
		 *	@brief	���\�[�X(�o�b�t�@)�𐶐�����
		 *	@param	device:Direct3D12�̃f�o�C�X
		 *	@param	properties:���\�[�X�̃q�[�v�v���p�e�B
		 *	@param	width:���\�[�X�̃T�C�Y
		 *	@param	flag:���\�[�X����I�v�V�����t���O
		 */
		void Buffer::Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const D3D12_RESOURCE_FLAGS flag) noexcept(false)
		{
			// ���\�[�X�̐ݒ�
			D3D12_RESOURCE_DESC resource_desc;
			resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;		// ���\�[�X�̎��(����͕K���o�b�t�@)
			resource_desc.Alignment = 0;									// �A���C�������g
			resource_desc.Width = width;									// ���\�[�X�̕�
			resource_desc.Height = 1;										// ���\�[�X�̍���(����͕K�v�����̕����m�ۂ��Ă���̂ŕK��1)
			resource_desc.DepthOrArraySize = 1;								// ���\�[�X�̐[��(�e�N�X�`�����Ɏg�p���镨�A����͕K��1)
			resource_desc.MipLevels = 1;									// �~�b�v�}�b�v�̃��x��(����͕K��1)
			resource_desc.Format = DXGI_FORMAT_UNKNOWN;						// ���\�[�X�f�[�^�t�H�[�}�b�g(R8G8B8A8��)(����͕K���s��)
			resource_desc.SampleDesc.Count = 1;								// �s�N�Z���P�ʂ̃}���`�T���v�����O��
			resource_desc.SampleDesc.Quality = 0;							// �}���`�T���v�����O�̕i�����x��
			resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;			// �e�N�X�`�����C�A�E�g�I�v�V����
			resource_desc.Flags = flag;										// ���\�[�X����I�v�V�����t���O
																
			// ���\�[�X(�o�b�t�@)�̐���
			ID3D12Resource* resource = nullptr;
			if (FAILED(device->CreateCommittedResource(
				&properties,						// �q�[�v�̐ݒ�
				D3D12_HEAP_FLAG_NONE,				// �q�[�v�I�v�V����(TODO:�K�؂ȃI�v�V������ݒ肷��)
				&resource_desc,						// ���\�[�X�̐ݒ�
				D3D12_RESOURCE_STATE_GENERIC_READ,	// ���\�[�X�̏��(TODO:�K�؂ȏ�Ԃ�ݒ肷��)
				nullptr,							// �N���A�J���[�̃f�t�H���g�l
				IID_PPV_ARGS(&resource))))
			{
				throw "���\�[�X(�o�b�t�@)�̍쐬�Ɏ��s���܂����B";
			}
			buffer.reset(resource);
		}
		
		/**
		 *	@brief	���\�[�X(�o�b�t�@)���J������
		 */
		void Buffer::Release() noexcept
		{
			if (buffer)
			{
				buffer.release()->Release();
			}
		}

		/**
		 *	@brief	���\�[�X(�o�b�t�@)���擾����
		 *	@return	���\�[�X�̎Q��
		 */
		ID3D12Resource& Buffer::Get() const noexcept
		{
			return *buffer;
		}
	};
};
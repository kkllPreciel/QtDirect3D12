/**
 *	@file		DepthStencilView.cpp
 *	@brief		�[�x�X�e���V���r���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/05/26
 *	@version	1.0
 */

// include
#include "DepthStencilView.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�R���X�g���N�^
		 */
		DepthStencilView::DepthStencilView() : depthStencil(nullptr), depthStencilViewHeap(nullptr)
		{

		}
		
		/**
		 *	@brief	�f�X�g���N�^
		 */
		DepthStencilView::~DepthStencilView()
		{
			Release();
		}

		/**
		 *	@brief	�[�x�X�e���V���r���[�̐������s��
		 *	@param	device:Direct3D12�̃f�o�C�X
		 *	@param	width:����
		 *	@param	height:�c��
		 */
		void DepthStencilView::Create(ID3D12Device* const device, unsigned int width, unsigned int height)
		{
			Release();

			// �f�B�X�N���v�^�[�q�[�v�̍쐬
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			if (FAILED(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&depthStencilViewHeap))))
			{
				throw "�[�x�X�e���V���r���[�p�f�B�X�N���v�^�[�q�[�v�̐����Ɏ��s���܂����B";
			}

			// �[�x�X�e���V���r���[�̐���
			{
				D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
				depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
				depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
				depthOptimizedClearValue.DepthStencil.Stencil = 0;

				// �q�[�v�̐ݒ�
				D3D12_HEAP_PROPERTIES properties;
				properties.Type = D3D12_HEAP_TYPE_UPLOAD;						// �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
				properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
				properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;	// �q�[�v�̃������v�[��(�s���ɐݒ�)
				properties.CreationNodeMask = 1;								// ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
				properties.VisibleNodeMask = 1;									// ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

				// ���\�[�X�̐ݒ�
				D3D12_RESOURCE_DESC resource_desc;
				resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	// ���\�[�X�̎��(�����2D�e�N�X�`��)
				resource_desc.Alignment = 0;									// �A���C�������g
				resource_desc.Width = width;									// ���\�[�X�̕�(����̓E�B���h�E�̃T�C�Y)
				resource_desc.Height = height;									// ���\�[�X�̍���(����̓E�B���h�E�̃T�C�Y)
				resource_desc.DepthOrArraySize = 1;								// ���\�[�X�̐[��(�e�N�X�`�����Ɏg�p���镨�A�����1)
				resource_desc.MipLevels = 0;									// �~�b�v�}�b�v�̃��x��(�����0)
				resource_desc.Format = DXGI_FORMAT_UNKNOWN;						// ���\�[�X�f�[�^�t�H�[�}�b�g(R8G8B8A8��)(����͕s��)
				resource_desc.SampleDesc.Count = 1;								// �s�N�Z���P�ʂ̃}���`�T���v�����O��
				resource_desc.SampleDesc.Quality = 0;							// �}���`�T���v�����O�̕i�����x��
				resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;			// �e�N�X�`�����C�A�E�g�I�v�V����
				resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	// ���\�[�X����I�v�V�����t���O(����͐[�x�X�e���V��)

				if (FAILED(device->CreateCommittedResource(
					&properties,
					D3D12_HEAP_FLAG_NONE,
					&resource_desc,
					D3D12_RESOURCE_STATE_DEPTH_WRITE,
					&depthOptimizedClearValue,
					IID_PPV_ARGS(&depthStencil))))
				{
					throw "�[�x�X�e���V���r���[�p���\�[�X�̐����Ɏ��s���܂����B";
				}

				D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
				depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
				depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

				device->CreateDepthStencilView(depthStencil, &depthStencilDesc, depthStencilViewHeap->GetCPUDescriptorHandleForHeapStart());
			}
		}

		/**
		 *	@brief	���\�[�X���J������
		 */
		void DepthStencilView::Release()
		{
			if (nullptr != depthStencil)
			{
				depthStencil->Release();
				depthStencil = nullptr;
			}

			if (nullptr != depthStencilViewHeap)
			{
				depthStencilViewHeap->Release();
				depthStencilViewHeap = nullptr;
			}
		}
	};
};
/**
 *	@file		Direct3D12Device.h
 *	@brief		Direct3D12�̃f�o�C�X�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/02/06
 *	@version	1.0
 */

#pragma once

// include
#include <wrl\client.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>

namespace Sein
{
	namespace Direct3D12
	{
		class VertexBuffer;
		class IndexBuffer;

		/**
		 *	@brief	Direct3D12�p�f�o�C�X�N���X(�X���b�v�`�F�C�����ꏏ)
		 */
		class Device
		{
		public:
			/** 
			 *	@brief	�R���X�g���N�^
			 */
			Device();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~Device();

			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			Device(const Device& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			Device& operator = (const Device& other) = delete;

			/**
			 *	@brief	�f�o�C�X�𐶐�����
			 *	@param	handle:�E�B���h�E�n���h��
			 *	@param	width:����
			 *	@param	height:�c��
			 */
			void Create(HWND handle, unsigned int width, unsigned int height);

			/**
			 *	@brief	�f�o�C�X���J������
			 */
			void Release();

			/**
			 *	@brief	�V�[�����J�n����
			 */
			void BeginScene();

			/**
			 *	@brief	�V�[�����I������
			 */
			void EndScene();

			/**
			 *	@brief	��ʂ��X�V����
			 */
			void Present();

			/**
			 *	@brief	�f�o�C�X���擾����
			 *	@return	�f�o�C�X�ւ̎Q��
			 */
			ID3D12Device& GetDevice() const;

		private:
			ID3D12Device*				device;					///< �f�o�C�X
			IDXGISwapChain3*			swapChain;				///< �X���b�v�`�F�C��
			ID3D12CommandQueue*			commandQueue;			///< �R�}���h�L���[
			ID3D12CommandAllocator*		commandAllocator;		///< �R�}���h�A���P�[�^�[
			ID3D12GraphicsCommandList*	commandList;			///< �R�}���h���X�g
			ID3D12DescriptorHeap*		descriptorHeap;			///< �f�B�X�N���v�^�[�q�[�v
			unsigned int				descriptorSize;			///< �f�B�X�N���v�^�[�̃T�C�Y

			// ��X�_�u���o�b�t�@�N���X�ֈړ�
			static const unsigned int	FrameCount = 2;					///< �t���[����(�_�u���o�b�t�@)
			ID3D12Resource*				renderTargetList[FrameCount];	///< �����_�[�^�[�Q�b�g�̃��X�g
			unsigned int				bufferIndex;					///< ���݂̃o�b�t�@�ԍ�

			// ��X�t�F���X�N���X�ֈړ�
			ID3D12Fence*				fence;		///< �t�F���X
			unsigned int				fenceIndex;	///< ���݂̃t�F���X�ԍ�
			HANDLE						fenceEvent;	///< �t�F���X�C�x���g�n���h��

			/**
			 *	@brief	�`��I���҂����s��
			 */
			void WaitForGpu();

			/**
			 *	@brief	�A�Z�b�g��ǂݍ���
			 *	@param	width:�E�B���h�E����
			 *	@param	height:�E�B���h�E�c��
			 */
			void LoadAssets(unsigned int width, unsigned int height);

			// ���[�g�V�O�l�`���֘A
			// ��X�ʃN���X�ֈړ�
#pragma region RootSignature

			ID3D12RootSignature*	rootSignature;	///< ���[�g�V�O�l�`��

#pragma endregion

			// ���_�o�b�t�@�֘A
			// ��X�ʃN���X�ֈړ�
#pragma region VertexBuffer
			/**
			 *	@brief	���_�t�H�[�}�b�g
			 */
			struct Vertex {
				DirectX::XMFLOAT3 position; ///< ���_���W
				DirectX::XMFLOAT4 color;	///< ���_�F
			};

			ID3D12Resource*				vertexBuffer;		///< ���_�o�b�t�@
			D3D12_VERTEX_BUFFER_VIEW	vertexBufferView;	///< ���_�o�b�t�@�̃r���[
#pragma endregion

			// �p�C�v���C���X�e�[�g�֘A
			// ��X�ʃN���X�ֈړ�
#pragma region PipelineState
			ID3D12PipelineState*	pipelineState;	///< �p�C�v���C���X�e�[�g

		public:
			/**
			 *	@brief	�`�悷��
			 *	@param	vertexBuffer:���_�o�b�t�@
			 *	@param	indexBuffer:���_�C���f�b�N�X�o�b�t�@
			 */
			void Render(const VertexBuffer& vertebBuffer, const IndexBuffer& indexBuffer);
#pragma endregion

			// �萔�o�b�t�@�֘A
			// ��X�ʃN���X�ֈړ�
#pragma region ConstantBuffer

			/**
			 *	@brief	�萔�o�b�t�@�p�\����
			 */
			struct ConstantBuffer
			{
				DirectX::XMFLOAT4X4 world;			///< ���[���h�s��(���E���)
				DirectX::XMFLOAT4X4 view;			///< �r���[�s��(�������)
				DirectX::XMFLOAT4X4 projection;		///< �v���W�F�N�V�����s��(�ˉe���)
			};

			ID3D12DescriptorHeap* cbvHeap;			///< �萔�o�b�t�@�p�f�B�X�N���v�^�[�q�[�v
			ID3D12Resource* constantBuffer;			///< �萔�o�b�t�@
			unsigned int* constantBufferDataBegin;	///< �萔�o�b�t�@(���\�[�X)�ւ̃|�C���^
			ConstantBuffer constantBufferData;		///< �萔�o�b�t�@�p�̃f�[�^

		private:
			/**
			 *	@brief	�萔���o�b�t�@���쐬����
			 */
			void CreateConstantBuffer();

#pragma endregion
		};
	};
};
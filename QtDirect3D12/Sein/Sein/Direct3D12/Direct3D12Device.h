/**
 *	@file		Direct3D12Device.h
 *	@brief		Direct3D12�̃f�o�C�X�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/02/06
 *	@version	1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
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
		class DepthStencilView;
		class Fence;
		class ConstantBuffer;
		class ShaderResourceBuffer;

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

			Fence*						fence;		///< �t�F���X

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
			struct ConstantBufferType
			{
				DirectX::XMFLOAT4X4 world;						///< ���[���h�s��(���E���)
				DirectX::XMFLOAT4X4 view;						///< �r���[�s��(�������)
				DirectX::XMFLOAT4X4 projection;					///< �v���W�F�N�V�����s��(�ˉe���)
			};

			std::unique_ptr<ID3D12DescriptorHeap>	cbvSrvHeap;	///< �萔�o�b�t�@�r���[�A�V�F�[�_�[���\�[�X�r���[�p�f�B�X�N���v�^�[�q�[�v
			std::unique_ptr<ConstantBuffer>			cbvBuffer;	///< �萔�o�b�t�@
			ConstantBufferType constantBufferData;				///< �萔�o�b�t�@�p�̃f�[�^

		private:
			/**
			 *	@brief	�萔�o�b�t�@���쐬����
			 */
			void CreateConstantBuffer();

#pragma endregion

			// �[�x�X�e���V���r���[�֘A
#pragma region DepthStencilView
		private:
			DepthStencilView* depthStencilView;	///< �[�x�X�e���V���r���[

			/**
			 *	@brief	�[�x�X�e���V���r���[���쐬����
			 *	@param	width:�E�B���h�E����
			 *	@param	height:�E�B���h�E�c��
			 */
			void CreateDepthStencilView(unsigned int width, unsigned int height);


#pragma endregion

			// �C���X�^���V���O�֘A
			// ��X�ʃN���X�ֈړ�
#pragma region Instancing
		private:
			/**
			 *	@brief	�C���X�^���V���O�p�\����
			 */
			struct InstanceBuffer
			{
				DirectX::XMFLOAT4X4 world;			///< ���[���h�s��(���E���)
			};

			const unsigned int INSTANCE_NUM = 5;								///< �C���X�^���X�̐�
			std::vector<InstanceBuffer>				instanceBufferData;			///< �e�C���X�^���X���̃f�[�^���X�g
			std::unique_ptr<ShaderResourceBuffer>	srBuffer;					///< �V�F�[�_�[���\�[�X�o�b�t�@

		private:
			/**
			 *	@brief	�C���X�^���X�o�b�t�@���쐬����
			 */
			void CreateInstanceBuffer();

#pragma endregion
		};
	};
};
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

namespace Sein
{
	namespace Direct3D12
	{
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
			 *	@brief	��ʂ��X�V����
			 */
			void Present();

		private:
			ID3D12Device*		device;			///< �f�o�C�X
			IDXGISwapChain3*	swapChain;		///< �X���b�v�`�F�C��
			ID3D12CommandQueue*	commandQueue;	///< �R�}���h�L���[
		};
	};
};
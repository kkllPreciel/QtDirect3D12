/**
 *	@file		DepthStencilView.h
 *	@brief		�[�x�X�e���V���r���[�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/05/26
 *	@version	1.0
 */

#pragma once

// include
#include <d3d12.h>

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�[�x�X�e���V���r���[�N���X
		 */
		class DepthStencilView
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			DepthStencilView();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~DepthStencilView();

			/**
			 *	@brief	�[�x�X�e���V���r���[�̐������s��
			 *	@param	device:Direct3D12�̃f�o�C�X
			 *	@param	width:����
			 *	@param	height:�c��
			 */
			void Create(ID3D12Device* const device, unsigned int width, unsigned int height);

			/**
			 *	@brief	�f�B�X�N���v�^�[�n���h�����擾����
			 *	@return	�f�B�X�N���v�^�[�n���h��
			 */
			D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const;

			/**
			 *	@brief	���\�[�X���J������
			 */
			void Release();

		private:
			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			DepthStencilView(const DepthStencilView& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			DepthStencilView& operator = (const DepthStencilView& other) = delete;

		private:
			ID3D12Resource*			depthStencil;			///< ���\�[�X
			ID3D12DescriptorHeap*	depthStencilViewHeap;	///< �f�B�X�N���v�^�[�q�[�v
		};
	};
};
/**
 *	@file		IndexBuffer.h
 *	@brief		���_�C���f�b�N�X�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/05/08
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
		 *	@brief	���_�C���f�b�N�X�o�b�t�@�N���X
		 */
		class IndexBuffer
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			IndexBuffer();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~IndexBuffer();

			/**
			 *	@brief	���_�C���f�b�N�X�o�b�t�@�̐������s��
			 *	@param	device:Direct3D12�̃f�o�C�X
			 */
			void Create(const ID3D12Device& device);

		private:
			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			IndexBuffer(const IndexBuffer& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			IndexBuffer& operator = (const IndexBuffer& other) = delete;

		private:
			ID3D12Resource*				buffer;	///< ���_�C���f�b�N�X�o�b�t�@
			D3D12_INDEX_BUFFER_VIEW		view;	///< ���_�C���f�b�N�X�o�b�t�@�̃r���[
		};
	};
};
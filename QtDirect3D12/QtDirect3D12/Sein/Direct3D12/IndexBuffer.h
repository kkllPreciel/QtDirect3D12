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
			 *	@param	size:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(�S���_�C���f�b�N�X���v�T�C�Y)
			 *	@param	indices:���_�C���f�b�N�X�f�[�^
			 *	@param	format:�f�[�^�t�H�[�}�b�g
			 */
			void Create(ID3D12Device* const device, const unsigned int size, const void* const indices, DXGI_FORMAT format);

			/**
			 *	@brief	�r���[���擾����
			 *	@return	�r���[�ւ̎Q��
			 */
			const D3D12_INDEX_BUFFER_VIEW& GetView() const;

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
/**
 *	@file		VertexBuffer.h
 *	@brief		���_�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/05/08
 *	@version	1.0
 */

#pragma once

// include
#include <d3d12.h>
#include <memory>

namespace Sein
{
	namespace Direct3D12
	{
		class Buffer;

		/**
		 *	@brief	���_�o�b�t�@�N���X
		 */
		class VertexBuffer
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			VertexBuffer();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~VertexBuffer();

			/**
			 *	@brief	���_�o�b�t�@�̐������s��
			 *	@param	device:Direct3D12�̃f�o�C�X
			 *	@param	size:���_�o�b�t�@�̃T�C�Y(�S���_���v�T�C�Y)
			 *	@param	stride:1���_�̃T�C�Y
			 *	@param	vertices:���_�f�[�^
			 */
			void Create(ID3D12Device* const device, const unsigned int size, const unsigned short stride, const void* const vertices);

			/**
			 *	@brief	�r���[���擾����
			 *	@return	�r���[�ւ̎Q��
			 */
			const D3D12_VERTEX_BUFFER_VIEW& GetView() const;			

		private:
			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			VertexBuffer(const VertexBuffer& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			VertexBuffer& operator = (const VertexBuffer& other) = delete;

		private:
			std::unique_ptr<Buffer>		buffer; ///< �o�b�t�@(���\�[�X)
			D3D12_VERTEX_BUFFER_VIEW	view;	///< ���_�o�b�t�@�̃r���[
		};
	};
};
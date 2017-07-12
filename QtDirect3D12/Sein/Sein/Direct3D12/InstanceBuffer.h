/**
 *	@file		InstanceBuffer.h
 *	@brief		�C���X�^���V���O�p�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/07/12
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
		 *	@brief	�C���X�^���X�p�o�b�t�@�N���X
		 */
		class InstanceBuffer
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			InstanceBuffer();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~InstanceBuffer();

			/**
			 *	@brief	�C���X�^���X�o�b�t�@�𐶐�����
			 *	@param	device:Direct3D12�̃f�o�C�X
			 */
			void Create(ID3D12Device* const device);

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
			InstanceBuffer(const InstanceBuffer& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			InstanceBuffer& operator = (const InstanceBuffer& other) = delete;

		private:
			ID3D12Resource*				buffer;	///< �C���X�^���X�o�b�t�@
			D3D12_VERTEX_BUFFER_VIEW	view;	///< �C���X�^���X�o�b�t�@�̃r���[
		};
	};
};
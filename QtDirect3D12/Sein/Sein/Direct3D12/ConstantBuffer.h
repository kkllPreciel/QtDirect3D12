/**
 *	@file		ConstantBuffer.h
 *	@brief		�萔�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/07/21
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
		 *	@brief	�萔�o�b�t�@�N���X
		 */
		class ConstantBuffer
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			ConstantBuffer();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~ConstantBuffer();

			/**
			 *	@brief	�萔�o�b�t�@�𐶐�����
			 *	@param	device:Direct3D12�̃f�o�C�X
			 *	@param	descriptor_handle:�f�B�X�N���v�^�[�n���h��
			 *	@param	size:�萔�o�b�t�@�̃T�C�Y
			 */
			void Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, const unsigned int size) noexcept(false);

			/**
			 *	@brief	�萔�o�b�t�@���J������
			 */
			void Release() noexcept;

			/**
			 *	@brief	�f�[�^�̃}�b�v���s��
			 *	@param	data:�}�b�v����f�[�^
			 *	@param	size:�}�b�v����f�[�^�̃T�C�Y
			 */
			void Map(const void* const data, const unsigned int size) noexcept(false);

		private:
			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			ConstantBuffer(const ConstantBuffer& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			ConstantBuffer& operator = (const ConstantBuffer& other) = delete;

		private:
			std::unique_ptr<Buffer> buffer; ///< �o�b�t�@(���\�[�X)
			void* buffer_pointer;			///< �o�b�t�@�ւ̃|�C���^
		};
	};
};
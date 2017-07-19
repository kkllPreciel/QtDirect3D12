/**
 *	@file		Buffer.h
 *	@brief		�o�b�t�@�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/07/19
 *	@version	1.0
 */

#pragma once

// include
#include <memory>

struct ID3D12Resource;

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�o�b�t�@�N���X
		 */
		class Buffer
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			Buffer();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~Buffer();

			/**
			 *	@brief	���\�[�X(�o�b�t�@)�𐶐�����
			 *	@param	device:Direct3D12�̃f�o�C�X
			 *	@param	properties:���\�[�X�̃q�[�v�v���p�e�B
			 *	@param	width:���\�[�X�̃T�C�Y
			 *	@param	flag:���\�[�X����I�v�V�����t���O
			 */
			void Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const D3D12_RESOURCE_FLAGS flag = D3D12_RESOURCE_FLAG_NONE) noexcept(false);

			/**
			 *	@brief	���\�[�X(�o�b�t�@)���J������
			 */
			void Release() noexcept;

			/**
			 *	@brief	���\�[�X(�o�b�t�@)���擾����
			 *	@return	���\�[�X�̎Q��
			 */
			ID3D12Resource& Get() const noexcept(false);

		private:
			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			Buffer(const Buffer& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			Buffer& operator = (const Buffer& other) = delete;

		private:
			// TODO:deleter���g�p����
			std::unique_ptr<ID3D12Resource> buffer; ///< �o�b�t�@
		};
	};
};
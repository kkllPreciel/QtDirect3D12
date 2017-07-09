#pragma once
/**
 *	@file		Fence.h
 *	@brief		�t�F���X�Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/07/05
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
		 *	@brief	�t�F���X�N���X
		 */
		class Fence
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			Fence();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~Fence();

			/**
			 *	@brief	�쐬����
			 *	@param	device:Direct3D12�̃f�o�C�X
			 */
			void Create(ID3D12Device* const device);

			/**
			 *	@brief	�ҋ@����
			 *	@param	commandQueue:�R�}���h�L���[
			 */
			void Wait(ID3D12CommandQueue* const commandQueue);

			/**
			 *	@brief	���\�[�X���J������
			 */
			void Release();

		private:
			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			Fence(const Fence& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			Fence& operator = (const Fence& other) = delete;

			ID3D12Fence*	fence;			///< �t�F���X
			unsigned int	index;			///< ���݂̔ԍ�
			HANDLE			eventHandle;	///< �C�x���g�n���h��
		};
	};
};
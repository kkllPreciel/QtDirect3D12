/**
 *	@file		Fence.cpp
 *	@brief		�t�F���X�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/07/05
 *	@version	1.0
 */

#include "Fence.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�R���X�g���N�^
		 */
		Fence::Fence() : fence(nullptr), index(0), eventHandle(nullptr)
		{

		}

		/**
		 *	@brief	�쐬����
		 *	@param	device:Direct3D12�̃f�o�C�X
		 */
		void Fence::Create(ID3D12Device* const device)
		{
			if (FAILED(device->CreateFence(
				index,					// �t�F���X�̏����l
				D3D12_FENCE_FLAG_NONE,	// �I�v�V�����̎w��(����͎w��Ȃ�)
				IID_PPV_ARGS(&fence))))
			{
				throw "�t�F���X�̐����Ɏ��s���܂����B";
			}

			++index;
			// �����҂��p�̃C�x���g�𐶐�
			if (nullptr == (eventHandle = CreateEvent(nullptr, false, false, nullptr)))
			{
				throw "�C�x���g�̐����Ɏ��s���܂����B";
				// throw HRESULT_FROM_WIN32(GetLastError());
			}
		}

		/**
		 *	@brief	�ҋ@����
		 *	@param	commandQueue:�R�}���h�L���[
		 */
		void Fence::Wait(ID3D12CommandQueue* const commandQueue)
		{
			// ���s����Ă���R�}���h���X�g������������
			// �t�F���X�Ɏw��̒l��ݒ肷��悤�ɂ���
			if (FAILED(commandQueue->Signal(fence, index)))
			{
				throw "�R�}���h�L���[�̃V�O�i���ݒ�Ɏ��s���܂����B";
			}

			// ���ɏ������I����Ă���ꍇ�͎��s���Ȃ�
			if (fence->GetCompletedValue() < index)
			{
				// �t�F���X�̒l���ݒ肵���l�ɂȂ�����
				// �w�肵���C�x���g�𔭍s������
				if (FAILED(fence->SetEventOnCompletion(index, eventHandle)))
				{
					throw "�t�F���X�̃C�x���g���s�ݒ�Ɏ��s���܂����B";
				}

				// �V�O�i�����(�C�x���g���s)�ɂȂ�܂őҋ@����
				WaitForSingleObjectEx(eventHandle, INFINITE, false);
			}

			// �t�F���X�̒l���X�V����
			++index;
		}
		
		/**
		 *	@brief	�f�X�g���N�^
		 */
		Fence::~Fence()
		{
			Release();
		}
		
		/**
		 *	@brief	���\�[�X���J������
		 */
		void Fence::Release()
		{
			if (nullptr != eventHandle)
			{
				CloseHandle(eventHandle);
				eventHandle = nullptr;
			}

			if (nullptr != fence)
			{
				fence->Release();
				fence = nullptr;
			}
		}
	};
};
/**
 *	@file		Direct3D12Device.cpp
 *	@brief		Direct3D12�f�o�C�X�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/02/07
 *	@version	1.0
 */

#if _DEBUG
#include <sstream>
#endif
#include <winerror.h>
#include "Direct3D12Device.h"

namespace Sein
{
	namespace Direct3D12
	{
		/** 
		 *	@brief	�R���X�g���N�^
		 */
		Device::Device() :
			device(nullptr), swapChain(nullptr), commandQueue(nullptr), commandAllocator(nullptr),
			commandList(nullptr), descriptorHeap(nullptr), descriptorSize(0), bufferIndex(0),
			fence(nullptr), fenceIndex(0), fenceEvent(nullptr)
		{
			for (auto i = 0; i < FrameCount; ++i)
			{
				renderTargetList[i] = nullptr;
			}
		}

		/**
		 *	@brief	�f�X�g���N�^
		 */
		Device::~Device()
		{
			Release();
		}

		/**
		 *	@brief	�f�o�C�X�𐶐�����
		 *	@param	handle:�E�B���h�E�n���h��
		 *	@param	width:����
		 *	@param	height:�c��
		 */
		void Device::Create(HWND handle, unsigned int width, unsigned int height)
		{
#if _DEBUG
			// �f�o�b�O���C���[��L���ɐݒ肷��
			Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
			if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
			{
				throw "�f�o�b�O���C���[�̐����Ɏ��s���܂����B";	
			}
			debugInterface->EnableDebugLayer();
#endif
			// �t�@�N�g���̐���
			// �A�_�v�^�[�̗񋓂Ɏg�p
			Microsoft::WRL::ComPtr<IDXGIFactory4> factory;

			if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
			{
				throw "DXGI�t�@�N�g���̐����Ɏ��s���܂����B";
			}

			// Direct3D12�̃f�o�C�X���쐬����
			{
#if 1
				// �f�o�C�X�𐶐�
				// �A�_�v�^�[��񋓂��쐬�����
				// ���ɂ���ă��������[�N��������������
				// �f�t�H���g�̃A�_�v�^�[���g�p���쐬����
				if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
				{
					throw "�f�o�C�X�̐����Ɏ��s���܂����B";
				}
#else
				Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;

				// �A�_�v�^�[(�r�f�I�J�[�h)��񋓂���
				for (auto i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pAdapter); ++i)
				{
					DXGI_ADAPTER_DESC1 desc;
					pAdapter->GetDesc1(&desc);

					// TODO:����
					if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
					{
						continue;
					}

					// �f�o�C�X���쐬
					if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
					{
						continue;
					}

#if _DEBUG
					std::wostringstream ostr;
					pAdapter->GetDesc1(&desc);
					ostr << "D3D12-capable hardware found:" << desc.Description << "(" << (desc.DedicatedVideoMemory >> 20) << "MB)\n";
					OutputDebugString(ostr.str().c_str());
#endif
				}

				// WARP�Ő�������
				if (nullptr == device)
				{
#if _DEBUG
					OutputDebugString(L"WARP software adapter requested.  Initializing...\n");
#endif
					if (FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter))))
					{
						throw "WARP�A�_�v�^�[�����݂��܂���B";
					}

					if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
					{
						throw "�f�o�C�X�̐����Ɏ��s���܂����B";
					}
				}
#endif
			}

			// �R�}���h�L���[�̍쐬
			// �R�}���h�L���[��GPU�֕`�施�߂��o���ꂽ�R�}���h���X�g��
			// �������s����
			// �܂����̃A�_�v�^�[(GPU)�Ɋ֘A�t�����Ă���
			{
				D3D12_COMMAND_QUEUE_DESC queueDesc = {};
				queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// �^�C���A�E�g������L���ɂ���
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// �����_�����O�֘A�̃R�}���h���X�g

				if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue))))
				{
					throw "�R�}���h�L���[�̐����Ɏ��s���܂����B";
				}
			}

			// �X���b�v�`�F�C���̍쐬
			// �R�}���h�L���[���w�肵�č쐬���� = �A�_�v�^�[���w�肵�č쐬����
			{
				DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
				swapChainDesc.Width = width;									// �E�B���h�E����
				swapChainDesc.Height = height;									// �E�B���h�E�c��
				swapChainDesc.BufferCount = FrameCount;							// �o�b�t�@�̐���2��(�t�����g�o�b�t�@���܂ނ炵�����A�����T���v����������ƃo�b�N�o�b�t�@�̐��ȋC������)
				swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// ���炭�o�b�t�@�̃t�H�[�}�b�g(4 �����A32 �r�b�g�����Ȃ�����)
				swapChainDesc.Scaling = DXGI_SCALING_NONE;						// ��ʃT�C�Y�ƃo�b�t�@�T�C�Y���������Ȃ����̊g�k����(�g��k���͍s��Ȃ�)
				swapChainDesc.SampleDesc.Quality = 0;							// �}���`�T���v�����O�̕i�����x��
				swapChainDesc.SampleDesc.Count = 1;								// �s�N�Z���P�ʂ̃}���`�T���v�����O��
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p�ړI�y��CPU�A�N�Z�X�I�v�V����(�����_�[�^�[�Q�b�g�Ƃ��Ďg�p)
				swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// �X���b�v�`�F�C���̓���I�v�V����(���[�h�ؑ։\�ɐݒ�)
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̃X���b�v�����w��(�o�b�N�o�b�t�@���f�B�X�v���C�ɕ\�����ꂽ��j������)

				Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwapChain;
				if (FAILED(factory->CreateSwapChainForHwnd(
					commandQueue,	// �R�}���h�L���[
					handle,			// �E�B���h�E�n���h��
					&swapChainDesc,	// �X���b�v�`�F�C���̐ݒ���
					nullptr,		// �t���X�N���[���X���b�v�`�F�C���̐ݒ�(�E�B���h�E���[�h�ō쐬����̂�nullptr)
					nullptr,		// TODO:����
					&pSwapChain)))
				{
					throw "�X���b�v�`�F�C���̐����Ɏ��s���܂����B";
				}
				
				if (FAILED(pSwapChain.Get()->QueryInterface(IID_PPV_ARGS(&swapChain))))
				{
					throw "IDXGISwapChain3�̐����Ɏ��s���܂����B";
				}

				// �o�b�N�o�b�t�@�̔ԍ����擾����
				bufferIndex = swapChain->GetCurrentBackBufferIndex();
			}

			// �R�}���h�A���P�[�^�[�̐���
			// �R�}���h�Ɏg�p����o�b�t�@�̈���m�ۂ��镨
			{
				if (FAILED(device->CreateCommandAllocator(
					D3D12_COMMAND_LIST_TYPE_DIRECT,	// �R�}���h�A���P�[�^�[�̎��(�����_�����O�֘A�̃R�}���h���X�g��ݒ�)
					IID_PPV_ARGS(&commandAllocator))))
				{
					throw "�R�}���h�A���P�[�^�[�̐����Ɏ��s���܂����B";
				}
			}

			// �R�}���h���X�g�̐���
			// �R�}���h�L���[�ɓn���R�}���h�̃��X�g
			{
				if (FAILED(device->CreateCommandList(
					0,									// �}���`�A�_�v�^�[(�}���`GPU)�̏ꍇ�Ɏg�p����A�_�v�^�[(GPU)�̎��ʎq(�P��Ȃ̂�0)
					D3D12_COMMAND_LIST_TYPE_DIRECT,		// �R�}���h���X�g�̎��(�����_�����O�֘A�̃R�}���h���X�g)
					commandAllocator,					// ���̃R�}���h���X�g�Ŏg�p����R�}���h�A���P�[�^�[
					nullptr,							// �R�}���h���X�g�̏����p�C�v���C�����(�_�~�[�̏����p�C�v���C�����w��)
					IID_PPV_ARGS(&commandList))))
				{
					throw "�R�}���h���X�g�̐����Ɏ��s���܂����B";
				}
			}

			// Alt + Enter�Ńt���X�N���[�����̋@�\�𖳌��ɐݒ�
			factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER);

			// �f�B�X�N���v�^�[�q�[�v�̍쐬
			// �f�B�X�N���v�^�[�̓o�b�t�@�̏��f�[�^(�e�N�X�`���o�b�t�@�A���_�o�b�t�@��)
			{
				D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
				rtvHeapDesc.NumDescriptors = FrameCount;				// �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�t�����g�o�b�t�@�A�o�b�N�o�b�t�@)
				rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		// ��ʂ̓����_�[�^�[�Q�b�g�r���[
				rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// �V�F�[�_�[����Q�Ƃ��Ȃ�

				if (FAILED(device->CreateDescriptorHeap(
					&rtvHeapDesc,	// �f�B�X�N���v�^�[�q�[�v�̐ݒ���
					IID_PPV_ARGS(&descriptorHeap
					))))
				{
					throw "�f�B�X�N���v�^�[�q�[�v�̐����Ɏ��s���܂����B";
				}

				// �����_�[�^�[�Q�b�g���̃f�B�X�N���v�^�[�̃T�C�Y���擾����
				descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			}

			// �f�B�X�N���v�^�[�̓o�^
			{
				D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

				// �t���[���o�b�t�@�����o�^����
				for (auto i = 0; i < FrameCount; ++i)
				{
					if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargetList[i]))))
					{
						throw "�o�b�N�o�b�t�@�̎擾�Ɏ��s���܂����B";
					}

					// �����_�[�^�[�Q�b�g�r���[���쐬����
					device->CreateRenderTargetView(
						renderTargetList[i],	// �����_�[ �^�[�Q�b�g��\��ID3D12Resource�ւ̃|�C���^
						nullptr,				// D3D12_RENDER_TARGET_VIEW_DESC�ւ̃|�C���^
						renderTargetViewHandle
					);

					renderTargetViewHandle.ptr += descriptorSize;
				}
			}

			// �t�F���X�̐���
			// Directx12�ł�GPU�̕`��̏I���҂��������ōs��Ȃ�(��������ꂸ��ʂ����������Ȃ�)
			// ���̂��ߓ�������邽�߂̃I�u�W�F�N�g(�t�F���X)���쐬����
			{
				if (FAILED(device->CreateFence(
					fenceIndex,				// �t�F���X�̏����l
					D3D12_FENCE_FLAG_NONE,	// �I�v�V�����̎w��(����͎w��Ȃ�)
					IID_PPV_ARGS(&fence))))
				{
					throw "�t�F���X�̐����Ɏ��s���܂����B";
				}

				++fenceIndex;

				// �����҂��p�̃C�x���g�𐶐�
				if (nullptr == (fenceEvent = CreateEvent(nullptr, false, false, nullptr)))
				{
					throw "�C�x���g�̐����Ɏ��s���܂����B";
					// throw HRESULT_FROM_WIN32(GetLastError());
				}

				// �`�揈�����s���Ă���\��������̂ŕ`��I���҂����s��
				WaitForGpu();
			}
		}

		/**
		 *	@brief	�f�o�C�X���J������
		 */
		void Device::Release()
		{
			// GPU�̕`��I���҂����s��
			WaitForGpu();

			CloseHandle(fenceEvent);

			fence->Release();

			for (auto i = 0; i < FrameCount; ++i)
			{
				renderTargetList[i]->Release();
			}

			descriptorHeap->Release();
			commandList->Release();
			commandAllocator->Release();
			commandQueue->Release();
			swapChain->Release();
			device->Release();
		}

		/**
		 *	@brief	��ʂ��X�V����
		 */
		void Device::Present()
		{

		}

		// ��X�ʃN���X�ֈړ�����
#pragma region Fence

		/**
		 *	@brief	�`��I���҂����s��
		 */
		void Device::WaitForGpu()
		{
			// ���s����Ă���R�}���h���X�g������������
			// �t�F���X�Ɏw��̒l��ݒ肷��悤�ɂ���
			if (FAILED(commandQueue->Signal(fence, fenceIndex)))
			{
				throw "�R�}���h�L���[�̃V�O�i���ݒ�Ɏ��s���܂����B";
			}

			// ���ɏ������I����Ă���ꍇ�͎��s���Ȃ�
			if (fence->GetCompletedValue() < fenceIndex)
			{
				// �t�F���X�̒l���ݒ肵���l�ɂȂ�����
				// �w�肵���C�x���g�𔭍s������
				if (FAILED(fence->SetEventOnCompletion(fenceIndex, fenceEvent)))
				{
					throw "�t�F���X�̃C�x���g���s�ݒ�Ɏ��s���܂����B";
				}

				// �V�O�i�����(�C�x���g���s)�ɂȂ�܂őҋ@����
				WaitForSingleObjectEx(fenceEvent, INFINITE, false);
			}

			// �t�F���X�̒l���X�V����
			++fenceIndex;
		}

#pragma endregion
	};
};
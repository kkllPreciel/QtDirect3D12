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
#include <d3dcompiler.h>
#include "Direct3D12Device.h"
#include "DepthStencilView.h"
#include "Fence.h"
#include "../Direct3D12/VertexBuffer.h"
#include "../Direct3D12/IndexBuffer.h"

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
			rootSignature(nullptr), pipelineState(nullptr), cbvHeap(nullptr), constantBuffer(nullptr),
			constantBufferDataBegin(nullptr), depthStencilView(nullptr), fence(nullptr)
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

				// �L�^���I������
				commandList->Close();
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
				fence = new Fence;
				fence->Create(device);

				// �`�揈�����s���Ă���\��������̂ŕ`��I���҂����s��
				WaitForGpu();
			}

			// �A�Z�b�g��ǂݍ���
			{
				LoadAssets(width, height);
			}
		}

		/**
		 *	@brief	�f�o�C�X���J������
		 */
		void Device::Release()
		{
			// GPU�̕`��I���҂����s��
			WaitForGpu();
			
			// �t�F���X�̍폜
			if (nullptr != fence)
			{
				fence->Release();
				delete fence;
				fence = nullptr;
			}

			pipelineState->Release();
			depthStencilView->Release();
			delete depthStencilView;
			depthStencilView = nullptr;
			constantBufferDataBegin = nullptr;
			constantBuffer->Unmap(0, nullptr);
			constantBuffer->Release();
			rootSignature->Release();

			for (auto i = 0; i < FrameCount; ++i)
			{
				renderTargetList[i]->Release();
			}

			cbvHeap->Release();
			descriptorHeap->Release();
			commandList->Release();
			commandAllocator->Release();
			commandQueue->Release();
			swapChain->Release();
			device->Release();
		}

		/**
		 *	@brief	�V�[�����J�n����
		 */
		void Device::BeginScene()
		{
			// �R�}���h�A���P�[�^�[�����Z�b�g
			if (FAILED(commandAllocator->Reset()))
			{
				throw "�R�}���h�A���P�[�^�[�̃��Z�b�g�Ɏ��s���܂����B";
			}

			// �R�}���h���X�g�����Z�b�g
			if (FAILED(commandList->Reset(commandAllocator, nullptr)))
			{
				throw "�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂����B";
			}

			// �o�b�N�o�b�t�@���`��^�[�Q�b�g�Ƃ��Ďg�p�ł���悤�ɂȂ�܂ő҂�
			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// �o���A�̓��\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = renderTargetList[bufferIndex];			// ���\�[�X�͕`��^�[�Q�b�g
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;			// �J�ڑO��Present
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		// �J�ڌ�͕`��^�[�Q�b�g
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			commandList->ResourceBarrier(1, &barrier);

			// �o�b�N�o�b�t�@��`��^�[�Q�b�g�Ƃ��Đݒ肷��
			// �f�o�C�X�֐[�x�X�e���V���r���[���o�C���h����
			D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			handle.ptr += bufferIndex * descriptorSize;
			D3D12_CPU_DESCRIPTOR_HANDLE depthHandle = depthStencilView->GetDescriptorHandle();
			commandList->OMSetRenderTargets(1, &handle, false, &depthHandle);

			// �o�b�N�o�b�t�@���N���A����
			const float Color[] = { 0.0f, 0.0f, 0.6f, 1.0f };
			commandList->ClearRenderTargetView(handle, Color, 0, nullptr);

			// �[�x�X�e���V���r���[���N���A����(�[�x�o�b�t�@�̂�)
			commandList->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		}

		/**
		 *	@brief	�V�[�����I������
		 */
		void Device::EndScene()
		{
			// �o�b�N�o�b�t�@�̕`�抮����҂��߂̃o���A��ݒu
			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// �o���A�̓��\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = renderTargetList[bufferIndex];			// ���\�[�X�͕`��^�[�Q�b�g
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// �J�ڑO�͕`��^�[�Q�b�g
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// �J�ڌ��Present
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			commandList->ResourceBarrier(1, &barrier);

			// �R�}���h���X�g���N���[�Y����
			commandList->Close();
		}

		/**
		 *	@brief	��ʂ��X�V����
		 */
		void Device::Present()
		{
			// �R�}���h���X�g�̎��s
			ID3D12CommandList* ppCommandLists[] = { commandList };
			commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			// �`��I���҂����s��
			WaitForGpu();

			// ��ʂ̍X�V
			if (FAILED(swapChain->Present(1, 0)))
			{
				throw "��ʂ̍X�V�Ɏ��s���܂����B";
			}

			// �o�b�t�@�ԍ����X�V
			bufferIndex = swapChain->GetCurrentBackBufferIndex();
		}

		// ��X�ʃN���X�ֈړ�����
#pragma region Fence

		/**
		 *	@brief	�`��I���҂����s��
		 */
		void Device::WaitForGpu()
		{
			fence->Wait(commandQueue);
		}

#pragma endregion

		/**
		 *	@brief	�A�Z�b�g��ǂݍ���
		 *	@param	width:�E�B���h�E����
		 *	@param	height:�E�B���h�E�c��
		 */
		void Device::LoadAssets(unsigned int width, unsigned int height)
		{
			// �萔�o�b�t�@�̐���
			{
				CreateConstantBuffer();
			}

			// �[�x�X�e���V���r���[�̍쐬
			{
				CreateDepthStencilView(width, height);
			}

			// ���[�g�V�O�l�`���̍쐬
			{
				// �f�B�X�N���v�^�[�����W�̐ݒ�
				D3D12_DESCRIPTOR_RANGE descriptorRange;
				descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;								// �f�B�X�N���v�^�[�̎��(�萔�o�b�t�@�r���[)
				descriptorRange.NumDescriptors = 1;															// �f�B�X�N���v�^�[�̐�
				descriptorRange.BaseShaderRegister = 0;														// �͈͓��̃x�[�X�V�F�[�_���W�X�^
				descriptorRange.RegisterSpace = 0;															// ���W�X�^���(TODO:���ׂ�)
				descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// ���[�g�V�O�l�`���J�n����̃f�B�X�N���v�^�̃I�t�Z�b�g?

				// ���[�g�p�����[�^�̐ݒ�
				D3D12_ROOT_PARAMETER rootParameter;
				rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;				// ���[�g�V�O�l�`���̃X���b�g�̎��(�f�B�X�N���v�^�e�[�u��)
				rootParameter.DescriptorTable.NumDescriptorRanges = 1;									// �f�B�X�N���v�^�[�����W�̐�
				rootParameter.DescriptorTable.pDescriptorRanges = &descriptorRange;						// �f�B�X�N���v�^�[�����W�̃|�C���^(����1���Ȃ�z��̐擪�|�C���^)
				rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;						// ���[�g�V�O�l�`���̃X���b�g�̓��e�ɃA�N�Z�X�ł���V�F�[�_�̎��(���_�V�F�[�_�̂�)

				// ���[�g�V�O�l�`���̐ݒ�
				D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
				rootSignatureDesc.NumParameters = 1;													// ���[�g�V�O�l�`���̃X���b�g��
				rootSignatureDesc.pParameters = &rootParameter;											// �X���b�g�̍\��?
				rootSignatureDesc.NumStaticSamplers = 0;												// �ÓI�T���v���[��
				rootSignatureDesc.pStaticSamplers = nullptr;											// �ÓI�T���v���[�ݒ�f�[�^�̃|�C���^
				rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT	// �I�v�V����(�`��Ɏg�p����)
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS							// �n���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS							// �h���C���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS						// �W�I���g���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;							// �s�N�Z���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~
				

				// ���[�g�V�O�l�`���̃V���A����
				Microsoft::WRL::ComPtr<ID3DBlob> signature;
				if (FAILED(D3D12SerializeRootSignature(
					&rootSignatureDesc,				// ���[�g�V�O�l�`���̐ݒ�
					D3D_ROOT_SIGNATURE_VERSION_1,	// ���[�g�V�O�l�`���̃o�[�W����
					&signature,						// �V���A���C�Y�������[�g�V�O�l�`���փA�N�Z�X���邽�߂̃C���^�[�t�F�C�X(�|�C���^)
					nullptr							// �V���A���C�U�̃G���[���b�Z�[�W�փA�N�Z�X���邽�߂̃C���^�[�t�F�C�X(�|�C���^)
				)))
				{
					throw "���[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂����B";
				}

				// ���[�g�V�O�l�`���̐���
				if (FAILED(device->CreateRootSignature(
					0,									// �}���`�A�_�v�^�[(�}���`GPU)�̏ꍇ�Ɏg�p����A�_�v�^�[(GPU)�̎��ʎq(�P��Ȃ̂�0)
					signature->GetBufferPointer(),		// �V���A�������ꂽ�V�O�l�`���ݒ�ւ̃|�C���^
					signature->GetBufferSize(),			// �������̃u���b�N�T�C�Y
					IID_PPV_ARGS(&rootSignature))))
				{
					throw "���[�g�V�O�l�`���̐����Ɏ��s���܂����B";
				}
			}

			// �p�C�v���C���X�e�[�g�̍쐬
			// �V�F�[�_�[���ꏏ�ɃR���p�C������
			// ��X�̓R���p�C���ς݂̃V�F�[�_�[���g�p����
			{
				Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
				Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

				// �R���p�C���I�v�V�����t���O��ݒ肷��
#if defined(_DEBUG)
				UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
				UINT compileFlags = 0;
#endif

				// ���_�V�F�[�_�[�̃R���p�C��
				if (FAILED(D3DCompileFromFile(
					L"shaders.hlsl",	// �V�F�[�_�[�t�@�C����
					nullptr,			// �V�F�[�_�[�}�N��(����͎g�p���Ȃ�)
					nullptr,			// �C���N���[�h�t�@�C������舵�����߂Ɏg�p����ID3DInclude�C���^�t�F�[�X�ւ̃|�C���^(����͎g�p���Ȃ�)
					"VSMain",			// �G���g���[�|�C���g�̊֐���
					"vs_5_0",			// �R���p�C���^�[�Q�b�g(����͒��_�V�F�[�_�[�ŃV�F�[�_�[���f��5)
					compileFlags,		// �R���p�C���I�v�V����
					0,					// �G�t�F�N�g�t�@�C���̃R���p�C���I�v�V����(����̓G�t�F�N�g�Ƃ��Ďg�p���Ȃ��̂�0)
					&vertexShader,		// �R���p�C�����ꂽ�R�[�h�փA�N�Z�X���邽�߂�ID3DBlob�C���^�t�F�[�X�̃|�C���^
					nullptr				// �R���p�C���G���[���b�Z�[�W�փA�N�Z�X���邽�߂�ID3DBlob�C���^�t�F�[�X�̃|�C���^
				)))
				{
					throw "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂����B";
				}

				// �s�N�Z���V�F�[�_�[�̃R���p�C��
				if (FAILED(D3DCompileFromFile(
					L"shaders.hlsl",	// �V�F�[�_�[�t�@�C����
					nullptr,			// �V�F�[�_�[�}�N��(����͎g�p���Ȃ�)
					nullptr,			// �C���N���[�h�t�@�C������舵�����߂Ɏg�p����ID3DInclude�C���^�t�F�[�X�ւ̃|�C���^(����͎g�p���Ȃ�)
					"PSMain",			// �G���g���[�|�C���g�̊֐���
					"ps_5_0",			// �R���p�C���^�[�Q�b�g(����̓s�N�Z���V�F�[�_�[�ŃV�F�[�_�[���f��5)
					compileFlags,		// �R���p�C���I�v�V����
					0,					// �G�t�F�N�g�t�@�C���̃R���p�C���I�v�V����(����̓G�t�F�N�g�Ƃ��Ďg�p���Ȃ��̂�0)
					&pixelShader,		// �R���p�C�����ꂽ�R�[�h�փA�N�Z�X���邽�߂�ID3DBlob�C���^�t�F�[�X�̃|�C���^
					nullptr				// �R���p�C���G���[���b�Z�[�W�փA�N�Z�X���邽�߂�ID3DBlob�C���^�t�F�[�X�̃|�C���^
				)))
				{
					throw "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂����B";
				}

				// ���_���̓��C�A�E�g
				D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
				{
					// ���_�f�[�^
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

					// �C���X�^���X�f�[�^
					{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
					{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
					{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
					{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
				};

				// ���X�^���C�U�[�X�e�[�g�̐ݒ�
				D3D12_RASTERIZER_DESC rasterizer_desc;
				rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;								// �O�p�`��`�悷��Ƃ��Ɏg�p����h��Ԃ����[�h(����͓h��Ԃ�)
				rasterizer_desc.CullMode = D3D12_CULL_MODE_NONE;								// �J�����O�̃��[�h(�������̃|���S����`�悵�Ȃ�)
				rasterizer_desc.FrontCounterClockwise = FALSE;									// �|���S���̕\���̔�����@(����͎��v���Ȃ�\)
				rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;							// �[�x�o�C�A�X(Z-Fighting�΍�?)
				rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;				// �[�x�o�C�A�X�̃N�����v�l
				rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;	// �X�΂��l�������[�x�o�C�A�X(�s�[�^�[�p�����ۑ΍�?)
				rasterizer_desc.DepthClipEnable = TRUE;											// Z�N���b�s���O���s����(����͍s��)
				rasterizer_desc.MultisampleEnable = FALSE;										// �}���`�T���v�����O�̃A���`�G�C���A�V���O���s����(����͂��Ȃ�)
				rasterizer_desc.AntialiasedLineEnable = FALSE;									// ���̃A���`�G�C���A�V���O���s����(����͂��Ȃ�)
				rasterizer_desc.ForcedSampleCount = 0;											// UAV�����_�����O�܂��̓��X�^���C�Y���ɋ��������T���v����(����͋������Ȃ�)
				rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;	// �ێ�I�ȃ��X�^���C�Y�t���O(����̓I�t)
				
				// �p�C�v���C���X�e�[�g�̐ݒ�Ɏg�p���钸�_�V�F�[�_�[�̃f�[�^�\�����쐬
				D3D12_SHADER_BYTECODE vs;
				vs.pShaderBytecode = vertexShader.Get()->GetBufferPointer();
				vs.BytecodeLength = vertexShader.Get()->GetBufferSize();

				// �p�C�v���C���X�e�[�g�̐ݒ�Ɏg�p����s�N�Z���V�F�[�_�[�̃f�[�^�\�����쐬
				D3D12_SHADER_BYTECODE ps;
				ps.pShaderBytecode = pixelShader.Get()->GetBufferPointer();
				ps.BytecodeLength = pixelShader.Get()->GetBufferSize();

				// �����_�[�^�[�Q�b�g�̃u�����h��Ԃ̐ݒ�
				const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTarggetBlendDesc = 
				{
					false,							// �u�����f�B���O��L���ɂ��邩(����͖���)
					false,							// �_�����Z��L���ɂ��邩(����͖���)
					D3D12_BLEND_ONE,				// RGB�l�s�N�Z���V�F�[�_�o�͂ɑ΂��Ď��s���鑀����w��
					D3D12_BLEND_ZERO,				// �����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���铮����w�肷��
					D3D12_BLEND_OP_ADD,				// RGB�܂��̓A���t�@�u�����f�B���O������w��(�\�[�X1�ƃ\�[�X2��ǉ�)
					D3D12_BLEND_ONE,				// �s�N�Z���V�F�[�_�o�͂���A���t�@�l�ɑ΂��Ď��s���铮����w��
					D3D12_BLEND_ZERO,				// �����_�[�^�[�Q�b�g�Ō��݂̃A���t�@�l�ɑ΂��Ď��s���铮��
					D3D12_BLEND_OP_ADD,				// RBG�܂��̓A���t�@�u�����f�B���O������w��(�\�[�X1�ƃ\�[�X2��ǉ�)
					D3D12_LOGIC_OP_NOOP,			// �����_�[�^�[�Q�b�g�ɐݒ肷��_�����Z?(�m�[�I�y���[�V�����A�������Ȃ�)
					D3D12_COLOR_WRITE_ENABLE_ALL	// 
				};

				// �u�����h��Ԃ̐ݒ�
				D3D12_BLEND_DESC blendDesc;
				blendDesc.AlphaToCoverageEnable = false;	// �A���t�@�g�D�J�o���b�W��L���ɂ��邩
				blendDesc.IndependentBlendEnable = false;	// ���������_�[�^�[�Q�b�g�ɓƗ������u�����h��L���ɂ��邩�ǂ������w�肵�܂�
				for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
					blendDesc.RenderTarget[i] = defaultRenderTarggetBlendDesc;

				// �[�x�X�e���V����Ԃ̐ݒ�
				D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
				depthStencilDesc.DepthEnable = true;							// �f�v�X�e�X�g��L���ɂ��邩?
				depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �[�x�X�e���V�� �o�b�t�@�[�ւ̏������݂��I���ɂ��܂�
				depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		// �[�x�f�[�^�������̐[�x�f�[�^�Ɣ�r����֐��ł��B�\�[�X�f�[�^���Ώۃf�[�^�����������ꍇ�A��r�ɍ��i���܂��B
				depthStencilDesc.StencilEnable = false;							// �X�e���V���e�X�g��L���ɂ��邩?

				// �}���`�T���v�����O�p�����[�^�[�̐ݒ�
				DXGI_SAMPLE_DESC sampleDesc = {};
				sampleDesc.Count = 1;	// �s�N�Z���P�ʂ̃}���`�T���v�����O�̐�

				// �p�C�v���C���X�e�[�g�̐ݒ�
				D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
				psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };	// ���̓��C�A�E�g�̍\��
				psoDesc.pRootSignature = rootSignature;										// ���[�g�V�O�l�`��
				psoDesc.RasterizerState = rasterizer_desc;									// ���X�^���C�U�̏��
				psoDesc.VS = vs;															// ���_�V�F�[�_�[�̍\��
				psoDesc.PS = ps;															// �s�N�Z���V�F�[�_�[�̍\��
				psoDesc.BlendState = blendDesc;												// �u�����h��Ԃ̍\��
				psoDesc.DepthStencilState = depthStencilDesc;								// �[�x�X�e���V����Ԃ̍\��
				psoDesc.SampleMask = UINT_MAX;												// �u�����h�̏�Ԃ̂��߂̃T���v���̃}�X�N
				psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;		// ���̓v���~�e�B�u(�O�p�`)
				psoDesc.NumRenderTargets = 1;												// �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g��
				psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;							// �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
				psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;									// �[�x�X�e���V���̃t�H�[�}�b�g
				psoDesc.SampleDesc = sampleDesc;											// �T���v�����O��Ԃ̍\��

				// �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
				if (FAILED(device->CreateGraphicsPipelineState(
					&psoDesc,
					IID_PPV_ARGS(&pipelineState))))
				{
					throw "�p�C�v���C���X�e�[�g�̐����Ɏ��s���܂����B";
				}
			}
		}

		/**
		 *	@brief	�`�悷��
		 *	@param	vertexBuffer:���_�o�b�t�@
		 *	@param	indexBuffer:���_�C���f�b�N�X�o�b�t�@
		 */
		void Device::Render(const VertexBuffer& vertebBuffer, const IndexBuffer& indexBuffer)
		{
			static float now = 0.0f;
			static float angle = DirectX::XM_PI / 180.0f;

			// ��]
			now += angle;

			// ���[���h�s����X�V
			DirectX::XMStoreFloat4x4(&(constantBufferData.world), DirectX::XMMatrixRotationY(now));

			// �r���[�s����쐬
			DirectX::XMVECTORF32 eye = { 0.0f, 10.0f, -30.5f, 0.0f };
			DirectX::XMVECTORF32 at = { 0.0f, 10.0f, 0.0f, 0.0f };
			DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
			DirectX::XMStoreFloat4x4(&(constantBufferData.view), DirectX::XMMatrixLookAtLH(eye, at, up));

			// �v���W�F�N�V�����s����쐬
			DirectX::XMStoreFloat4x4(&(constantBufferData.projection), DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 3.0f, 600.0f / 400.0f, 0.1f, 1000.0f));

			// �萔�o�b�t�@���X�V
			std::memcpy(constantBufferDataBegin, &constantBufferData, sizeof(ConstantBuffer));

			// �r���[�|�[�g�̍쐬
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = 600;
			viewport.Height = 400;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;

			// �V�U�[��`(�V�U�[�e�X�g)�̍쐬
			D3D12_RECT scissor;
			scissor.left = 0;
			scissor.top = 0;
			scissor.right = 600;
			scissor.bottom = 400;

			// �p�C�v���C���X�e�[�g�̐ݒ�(�؂�ւ��Ȃ��ꍇ�́A�R�}���h���X�g���Z�b�g���ɐݒ�\)
			commandList->SetPipelineState(pipelineState);

			// �O���t�B�b�N�X�p�C�v���C���̃��[�g�V�O�l�`����ݒ肷��
			commandList->SetGraphicsRootSignature(rootSignature);

			// �`��Ɏg�p����f�B�X�N���v�^�[�q�[�v��ݒ�
			commandList->SetDescriptorHeaps(1, &cbvHeap);

			// �r���[�|�[�g�̐ݒ�
			commandList->RSSetViewports(1, &viewport);

			// �V�U�[��`(�V�U�[�e�X�g)�̐ݒ�
			commandList->RSSetScissorRects(1, &scissor);

			// �v���~�e�B�u�g�|���W�[�̐ݒ�
			//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
			commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// ���_�o�b�t�@�r���[�̐ݒ�
			commandList->IASetVertexBuffers(0, 1, &(vertebBuffer.GetView()));

			// ���_�C���f�b�N�X�r���[�̐ݒ�
			commandList->IASetIndexBuffer(&(indexBuffer.GetView()));

			// �f�B�X�N���v�[�^�q�[�v�e�[�u����ݒ�
			commandList->SetGraphicsRootDescriptorTable(0, cbvHeap->GetGPUDescriptorHandleForHeapStart());

			// �`��R�}���h�̐���
			// TODO:���_�C���f�b�N�X���g�p���ĕ`�悷��
			commandList->DrawIndexedInstanced(321567, 1, 0, 0, 0);
			//commandList->DrawInstanced(88402, 1, 0, 0);
			//commandList->DrawInstanced(3, 1, 0, 0);
		}

		/**
		 *	@brief	�f�o�C�X���擾����
		 *	@return	�f�o�C�X�ւ̎Q��
		 */
		ID3D12Device& Device::GetDevice() const
		{
			return *device;
		}

		// ��X�ʃN���X�ֈړ�����
#pragma region ConstantBuffer

		/**
		 *	@brief	�萔���o�b�t�@���쐬����
		 */
		void Device::CreateConstantBuffer()
		{
			// �萔�o�b�t�@�p�f�B�X�N���v�^�[�q�[�v�𐶐�
			{
				D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
				cbvHeapDesc.NumDescriptors = 1;									// �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�萔�o�b�t�@)
				cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;		// �萔�o�b�t�@ or �V�F�[�_�[���\�[�X(�e�N�X�`��) or �����_���A�N�Z�X �̂ǂꂩ�̃q�[�v
				cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_�[����A�N�Z�X��

				if (device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&cbvHeap)))
				{
					throw "�萔�o�b�t�@�p�f�B�X�N���v�^�[�q�[�v�̐����Ɏ��s���܂����B";
				}
			}

			// �萔�o�b�t�@�𐶐�
			{
				// �q�[�v�̐ݒ�
				D3D12_HEAP_PROPERTIES properties;
				properties.Type = D3D12_HEAP_TYPE_UPLOAD;						// �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
				properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
				properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;	// �q�[�v�̃������v�[��(�s���ɐݒ�)
				properties.CreationNodeMask = 1;								// ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
				properties.VisibleNodeMask = 1;									// ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

				// ���\�[�X�̐ݒ�
				D3D12_RESOURCE_DESC resource_desc;
				resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;		// ���\�[�X�̎��(����̓o�b�t�@)
				resource_desc.Alignment = 0;									// �A���C�������g
				resource_desc.Width = sizeof(ConstantBuffer);					// ���\�[�X�̕�(����͒萔�o�b�t�@�̃T�C�Y)
				resource_desc.Height = 1;										// ���\�[�X�̍���(����͒萔�o�b�t�@���̕����m�ۂ��Ă���̂�1)
				resource_desc.DepthOrArraySize = 1;								// ���\�[�X�̐[��(�e�N�X�`�����Ɏg�p���镨�A�����1)
				resource_desc.MipLevels = 1;									// �~�b�v�}�b�v�̃��x��(�����1)
				resource_desc.Format = DXGI_FORMAT_UNKNOWN;						// ���\�[�X�f�[�^�t�H�[�}�b�g(R8G8B8A8��)(����͕s��)
				resource_desc.SampleDesc.Count = 1;								// �s�N�Z���P�ʂ̃}���`�T���v�����O��
				resource_desc.SampleDesc.Quality = 0;							// �}���`�T���v�����O�̕i�����x��
				resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;			// �e�N�X�`�����C�A�E�g�I�v�V����
				resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;					// ���\�[�X����I�v�V�����t���O(����͖���)

				// �萔�o�b�t�@�p���\�[�X�̐���(�q�[�v�������ɐ��������)
				if (FAILED(device->CreateCommittedResource(
					&properties,						// �q�[�v�̐ݒ�
					D3D12_HEAP_FLAG_NONE,				// �q�[�v�I�v�V����(�ݒ�Ȃ�)
					&resource_desc,						// ���\�[�X�̐ݒ�
					D3D12_RESOURCE_STATE_GENERIC_READ,	// ���\�[�X�̏��
					nullptr,							// �N���A�J���[�̃f�t�H���g�l
					IID_PPV_ARGS(&constantBuffer))))
				{
					throw "�萔�o�b�t�@�p���\�[�X�̍쐬�Ɏ��s���܂����B";
				}

				// �萔�o�b�t�@�r���[�̐ݒ�
				D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};
				constantBufferViewDesc.BufferLocation = constantBuffer->GetGPUVirtualAddress();		// �o�b�t�@�̃A�h���X
				constantBufferViewDesc.SizeInBytes = (sizeof(ConstantBuffer) + 255) & ~255;			// �萔�o�b�t�@��256�o�C�g�ŃA���C�������g����Ă��Ȃ���΂Ȃ�Ȃ�

				// �萔�o�b�t�@�r���[�𐶐�
				device->CreateConstantBufferView(&constantBufferViewDesc, cbvHeap->GetCPUDescriptorHandleForHeapStart());

				// �}�b�v�B�A�v���P�[�V�����I���܂ŃA���}�b�v���Ȃ�
				if (FAILED(constantBuffer->Map(
					0,									// �T�u���\�[�X�̃C���f�b�N�X�ԍ�
					nullptr,							// CPU����A�N�Z�X���郁�����͈̔�(nullptr�͑S�̈�ɃA�N�Z�X����)
					reinterpret_cast<void**>(&constantBufferDataBegin))))	// ���\�[�X�f�[�^�ւ̃|�C���^
				{
					throw "�萔�o�b�t�@�p���\�[�X�ւ̃|�C���^�̎擾�Ɏ��s���܂����B";
				}

				// �萔�o�b�t�@�f�[�^�̏�����
				auto aspect = static_cast<float>(600) / static_cast<float>(400);
				DirectX::XMStoreFloat4x4(&(constantBufferData.world), DirectX::XMMatrixIdentity());
				DirectX::XMStoreFloat4x4(&(constantBufferData.view), DirectX::XMMatrixIdentity());
				DirectX::XMStoreFloat4x4(&(constantBufferData.projection), DirectX::XMMatrixIdentity());
				std::memcpy(constantBufferDataBegin, &constantBufferData, sizeof(ConstantBuffer));
			}
		}
#pragma endregion

		// �[�x�X�e���V���r���[�֘A
#pragma region DepthStencliView
		/**
		 *	@brief	�[�x�X�e���V���r���[���쐬����
		 *	@param	width:�E�B���h�E����
		 *	@param	height:�E�B���h�E�c��
		 */
		void Device::CreateDepthStencilView(unsigned int width, unsigned int height)
		{
			depthStencilView = new DepthStencilView();
			depthStencilView->Create(device, width, height);
		}
#pragma endregion
	};
};
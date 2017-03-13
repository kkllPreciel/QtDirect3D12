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
		Device::Device() : device(nullptr), swapChain(nullptr)
		{

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
				swapChainDesc.BufferCount = 2;									// �o�b�t�@�̐���2��(�t�����g�o�b�t�@���܂ނ炵�����A�����T���v����������ƃo�b�N�o�b�t�@�̐��ȋC������)
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
			}

			// �`��R�}���h�֘A
			// �R�}���h�A���P�[�^����

			// �`��ΏہE�Ǘ�
			// �t�F���X����
			// �f�B�X�N���v�^�[�q�[�v�̍쐬
			// �����_�[�^�[�Q�b�g�r���[�̍쐬
		}

		/**
		 *	@brief	�f�o�C�X���J������
		 */
		void Device::Release()
		{

		}

		/**
		 *	@brief	��ʂ��X�V����
		 */
		void Device::Present()
		{

		}
	};
};
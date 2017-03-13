/**
 *	@file		Direct3D12Device.h
 *	@brief		Direct3D12のデバイスに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/02/06
 *	@version	1.0
 */

#pragma once

// include
#include <wrl\client.h>
#include <d3d12.h>
#include <dxgi1_4.h>

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	Direct3D12用デバイスクラス(スワップチェインも一緒)
		 */
		class Device
		{
		public:
			/** 
			 *	@brief	コンストラクタ
			 */
			Device();

			/**
			 *	@brief	デストラクタ
			 */
			~Device();

			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			Device(const Device& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			Device& operator = (const Device& other) = delete;

			/**
			 *	@brief	デバイスを生成する
			 *	@param	handle:ウィンドウハンドル
			 *	@param	width:横幅
			 *	@param	height:縦幅
			 */
			void Create(HWND handle, unsigned int width, unsigned int height);

			/**
			 *	@brief	デバイスを開放する
			 */
			void Release();

			/**
			 *	@brief	画面を更新する
			 */
			void Present();

		private:
			ID3D12Device*		device;			///< デバイス
			IDXGISwapChain3*	swapChain;		///< スワップチェイン
			ID3D12CommandQueue*	commandQueue;	///< コマンドキュー
		};
	};
};
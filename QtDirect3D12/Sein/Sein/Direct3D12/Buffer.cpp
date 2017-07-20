/**
 *	@file		Buffer.cpp
 *	@brief		バッファに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/07/19
 *	@version	1.0
 */

// include
#include "Buffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	コンストラクタ
		 */
		Buffer::Buffer() : buffer(nullptr)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		Buffer::~Buffer()
		{
			Release();
		}

		/**
		 *	@brief	リソース(バッファ)を生成する
		 *	@param	device:Direct3D12のデバイス
		 *	@param	properties:リソースのヒーププロパティ
		 *	@param	width:リソースのサイズ
		 *	@param	flag:リソース操作オプションフラグ
		 */
		void Buffer::Create(ID3D12Device* const device, const D3D12_HEAP_PROPERTIES& properties, const UINT64 width, const D3D12_RESOURCE_FLAGS flag) noexcept(false)
		{
			// リソースの設定
			D3D12_RESOURCE_DESC resource_desc;
			resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;		// リソースの種別(今回は必ずバッファ)
			resource_desc.Alignment = 0;									// アラインメント
			resource_desc.Width = width;									// リソースの幅
			resource_desc.Height = 1;										// リソースの高さ(今回は必要数分の幅を確保しているので必ず1)
			resource_desc.DepthOrArraySize = 1;								// リソースの深さ(テクスチャ等に使用する物、今回は必ず1)
			resource_desc.MipLevels = 1;									// ミップマップのレベル(今回は必ず1)
			resource_desc.Format = DXGI_FORMAT_UNKNOWN;						// リソースデータフォーマット(R8G8B8A8等)(今回は必ず不明)
			resource_desc.SampleDesc.Count = 1;								// ピクセル単位のマルチサンプリング数
			resource_desc.SampleDesc.Quality = 0;							// マルチサンプリングの品質レベル
			resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;			// テクスチャレイアウトオプション
			resource_desc.Flags = flag;										// リソース操作オプションフラグ
																
			// リソース(バッファ)の生成
			ID3D12Resource* resource = nullptr;
			if (FAILED(device->CreateCommittedResource(
				&properties,						// ヒープの設定
				D3D12_HEAP_FLAG_NONE,				// ヒープオプション(TODO:適切なオプションを設定する)
				&resource_desc,						// リソースの設定
				D3D12_RESOURCE_STATE_GENERIC_READ,	// リソースの状態(TODO:適切な状態を設定する)
				nullptr,							// クリアカラーのデフォルト値
				IID_PPV_ARGS(&resource))))
			{
				throw "リソース(バッファ)の作成に失敗しました。";
			}
			buffer.reset(resource);
		}
		
		/**
		 *	@brief	リソース(バッファ)を開放する
		 */
		void Buffer::Release() noexcept
		{
			if (buffer)
			{
				buffer.release()->Release();
			}
		}

		/**
		 *	@brief	リソース(バッファ)を取得する
		 *	@return	リソースの参照
		 */
		ID3D12Resource& Buffer::Get() const noexcept
		{
			return *buffer;
		}
	};
};
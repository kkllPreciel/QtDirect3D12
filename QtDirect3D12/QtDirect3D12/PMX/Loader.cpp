/**
 *	@file		Loader.cpp
 *	@brief		Pmx読み込みに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/04/30
 *	@version	1.0
 */

#include <filesystem>
#include <fstream>
#include "Loader.h"

namespace Sein
{
	namespace Pmx
	{
		// アラインメントを1バイトに設定
#pragma pack( push, 1 )
		/**
		 *	@brief	Pmxヘッダデータ構造体
		 */
		struct Header
		{
			char	signature[4];	///< シグネチャ
			float	version;		///< バージョン
			char	globalsCount;	///< エンコード方式、追加UV数などの

		};
		// アラインメントをデフォルトの設定に戻す
#pragma pack(pop)

		/**
		 *	@brief	コンストラクタ
		 */
		Loader::Loader() : size(0), buffer(nullptr)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		Loader::~Loader()
		{
			if (nullptr != buffer)
			{
				delete buffer;
				buffer = nullptr;
			}
		}

		/**
		 *	@brief	読み込みを行う
		 *	@param	filePath:読み込みを行うPmxファイルのパス
		 */
		void Loader::Load(std::string filePath)
		{
			// ファイル内容の読み込み
			{
				namespace fs = std::experimental::filesystem;

				// ファイルが存在しない
				if (false == fs::exists(filePath))
				{
					return;
				}

				std::ifstream ifs(filePath, std::ios::in | std::ios::binary);

				ifs.seekg(0, std::fstream::end);
				unsigned int end = ifs.tellg();

				// 先頭に戻るために一度clear()をかける。
				// これをしないと次のseekg()でコケるときがある。
				ifs.clear();

				ifs.seekg(0, std::fstream::beg);
				size = end - ifs.tellg();

				// バッファの確保
				buffer = new char[size + 1];

				ifs.read(buffer, size);
				ifs.close();
			}

			// モデルデータ読み込み

			// ファイルサイズがヘッダサイズ未満

			// ヘッダ読み込み

			// 違法データチェック
		}
	};
};
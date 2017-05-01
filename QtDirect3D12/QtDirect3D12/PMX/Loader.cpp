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
		/**
		 *	@brief	コンストラクタ
		 */
		Loader::Loader() : size(0)
		{

		}

		/**
		 *	@brief	デストラクタ
		 */
		Loader::~Loader()
		{

		}

		/**
		 *	@brief	読み込みを行う
		 *	@param	filePath:読み込みを行うPmxファイルのパス
		 */
		void Loader::Load(std::string filePath)
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
			unsigned int size = end - ifs.tellg();

			// ファイルサイズがヘッダサイズ未満

			// ヘッダ読み込み

			// 違法データチェック
		}
	};
};
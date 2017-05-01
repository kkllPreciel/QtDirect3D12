/**
 *	@file		Loeader.h
 *	@brief		Pmx読み込みに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/04/30
 *	@version	1.0
 */

#pragma once

// include
#include <string>

namespace Sein
{
	namespace Pmx
	{
		/**
		 *	@brief	Pmx読み込み用クラス
		 */
		class Loader
		{
		public:
			/**
			 *	@brief	コンストラクタ
			 */
			Loader();

			/**
			 *	@brief	デストラクタ
			 */
			~Loader();

			/**
			 *	@brief	読み込みを行う
			 *	@param	filePath:読み込みを行うPmxファイルのパス
			 */
			void Load(std::string filePath);

		private:
			/**
			 *	@brief	コピーコンストラクタ
			 *	@param	other:コピー元のインスタンス
			 */
			Loader(const Loader& other) = delete;

			/**
			 *	@brief	代入演算子オペレータ
			 *	@param	other:代入元のインスタンス
			 *	@return	代入後のインスタンス
			 */
			Loader& operator = (const Loader& other) = delete;

		private:
			char*			buffer;	///< バッファ
			unsigned int	size;	///< ファイルサイズ
		};
	};
};
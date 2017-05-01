/**
 *	@file		Loeader.h
 *	@brief		Pmx�ǂݍ��݂Ɋւ���w�b�_�t�@�C��
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
		 *	@brief	Pmx�ǂݍ��ݗp�N���X
		 */
		class Loader
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			Loader();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~Loader();

			/**
			 *	@brief	�ǂݍ��݂��s��
			 *	@param	filePath:�ǂݍ��݂��s��Pmx�t�@�C���̃p�X
			 */
			void Load(std::string filePath);

		private:
			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			Loader(const Loader& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			Loader& operator = (const Loader& other) = delete;

		private:
			char*			buffer;	///< �o�b�t�@
			unsigned int	size;	///< �t�@�C���T�C�Y
		};
	};
};
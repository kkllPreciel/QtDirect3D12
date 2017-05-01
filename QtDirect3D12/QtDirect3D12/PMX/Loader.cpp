/**
 *	@file		Loader.cpp
 *	@brief		Pmx�ǂݍ��݂Ɋւ��鏈�����s���v���O�����\�[�X
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
		 *	@brief	�R���X�g���N�^
		 */
		Loader::Loader() : size(0)
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
		 */
		Loader::~Loader()
		{

		}

		/**
		 *	@brief	�ǂݍ��݂��s��
		 *	@param	filePath:�ǂݍ��݂��s��Pmx�t�@�C���̃p�X
		 */
		void Loader::Load(std::string filePath)
		{
			namespace fs = std::experimental::filesystem;

			// �t�@�C�������݂��Ȃ�
			if (false == fs::exists(filePath))
			{
				return;
			}

			std::ifstream ifs(filePath, std::ios::in | std::ios::binary);

			ifs.seekg(0, std::fstream::end);
			unsigned int end = ifs.tellg();

			// �擪�ɖ߂邽�߂Ɉ�xclear()��������B
			// ��������Ȃ��Ǝ���seekg()�ŃR�P��Ƃ�������B
			ifs.clear();

			ifs.seekg(0, std::fstream::beg);
			unsigned int size = end - ifs.tellg();

			// �t�@�C���T�C�Y���w�b�_�T�C�Y����

			// �w�b�_�ǂݍ���

			// ��@�f�[�^�`�F�b�N
		}
	};
};
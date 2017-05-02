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
		// �A���C�������g��1�o�C�g�ɐݒ�
#pragma pack( push, 1 )
		/**
		 *	@brief	Pmx�w�b�_�f�[�^�\����
		 */
		struct Header
		{
			char	signature[4];	///< �V�O�l�`��
			float	version;		///< �o�[�W����
			char	globalsCount;	///< �G���R�[�h�����A�ǉ�UV���Ȃǂ�

		};
		// �A���C�������g���f�t�H���g�̐ݒ�ɖ߂�
#pragma pack(pop)

		/**
		 *	@brief	�R���X�g���N�^
		 */
		Loader::Loader() : size(0), buffer(nullptr)
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
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
		 *	@brief	�ǂݍ��݂��s��
		 *	@param	filePath:�ǂݍ��݂��s��Pmx�t�@�C���̃p�X
		 */
		void Loader::Load(std::string filePath)
		{
			// �t�@�C�����e�̓ǂݍ���
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
				size = end - ifs.tellg();

				// �o�b�t�@�̊m��
				buffer = new char[size + 1];

				ifs.read(buffer, size);
				ifs.close();
			}

			// ���f���f�[�^�ǂݍ���

			// �t�@�C���T�C�Y���w�b�_�T�C�Y����

			// �w�b�_�ǂݍ���

			// ��@�f�[�^�`�F�b�N
		}
	};
};
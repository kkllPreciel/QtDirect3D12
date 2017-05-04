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
			char	globalsCount;	///< �t�@�C���S�̂ł̐ݒ���̌�(�G���R�[�h����, �ǉ�UV����)
			char	globals[8];		///< �t�@�C���S�̂ł̐ݒ���
		};
		// �A���C�������g���f�t�H���g�̐ݒ�ɖ߂�
#pragma pack(pop)

		/**
		 *	@brief	�R���X�g���N�^
		 */
		Loader::Loader() : size(0), buffer(nullptr), header(nullptr)
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

			if (nullptr != header)
			{
				delete header;
				header = nullptr;
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
					throw "Pmx�t�@�C�������݂��܂���B";
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
			{
				// �w�b�_�ǂݍ���
				{
					// �t�@�C���T�C�Y���w�b�_�T�C�Y����
					if (size < sizeof(Header))
					{
						throw "��@�ȃf�[�^��Pmx�t�@�C���ł��B";
					}

					header = new Header;
					std::memcpy(header, buffer, sizeof(Header));

					// ��@�f�[�^�`�F�b�N
					// �V�O�l�`�����uPMX �v�łȂ�
					// �o�[�W�������u2.0�v���u2.1�v�łȂ�
				}

				// ���f�����ǂݍ���
				{
					char* buffer = this->buffer + sizeof(Header);

					std::string tes = "����������";

					// ���f����
					unsigned int nameSize = 0;
					std::memcpy(&nameSize, buffer, sizeof(nameSize));
					std::string name(buffer + sizeof(nameSize), buffer + sizeof(nameSize) + nameSize);

					// �p��̃��f����

					// �R�����g

					// �p��̃R�����g
					int i = 0;
				}
			}

			{

			}
		}
	};
};
/**
 *	@file		Loader.cpp
 *	@brief		Pmx�ǂݍ��݂Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/04/30
 *	@version	1.0
 */

#include <filesystem>
#include <fstream>
#include <DirectXMath.h>
#include "Loader.h"

namespace Sein
{
	namespace Pmx
	{
		// �A���C�������g��1�o�C�g�ɐݒ�
#pragma pack(push, 1)
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

		// �A���C�������g��1�o�C�g�ɐݒ�
#pragma pack(push, 1)
		/**
		 *	@brief	Pmx���_�f�[�^�\����
		 */
		struct Vertex
		{
			DirectX::XMFLOAT3	position;	///< ���W
			DirectX::XMFLOAT3	normal;		///< �@��
			DirectX::XMFLOAT2	uv;			///< UV���W
		};
		// �A���C�������g���f�t�H���g�̐ݒ�ɖ߂�
#pragma pack(pop)

		/**
		 *	@brief	�R���X�g���N�^
		 */
		Loader::Loader() : size(0), buffer(nullptr), header(nullptr), vertexCount(0), vertices(nullptr), indexCount(0), indices(nullptr)
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

			if (nullptr != vertices)
			{
				delete[] vertices;
				vertices = nullptr;
			}

			if (nullptr != indices)
			{
				delete[] indices;
				indices = nullptr;
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
				char* buffer = this->buffer;

				// �w�b�_�ǂݍ���
				{
					// �t�@�C���T�C�Y���w�b�_�T�C�Y����
					if (size < sizeof(Header))
					{
						throw "��@�ȃf�[�^��Pmx�t�@�C���ł��B";
					}

					header = new Header;
					std::memcpy(header, buffer, sizeof(Header));
					buffer = buffer + sizeof(Header);

					// ��@�f�[�^�`�F�b�N
					// �V�O�l�`�����uPMX �v�łȂ�
					// �o�[�W�������u2.0�v���u2.1�v�łȂ�
				}

				// ���f�����ǂݍ���
				{
					// ���f����
					unsigned int nameSize = 0;
					std::memcpy(&nameSize, buffer, sizeof(nameSize));
					std::string name(buffer + sizeof(nameSize), buffer + sizeof(nameSize) + nameSize);
					buffer = buffer + sizeof(nameSize) + nameSize;

					// �p��̃��f����
					unsigned int englishNameSize = 0;
					std::memcpy(&englishNameSize, buffer, sizeof(englishNameSize));
					std::string englishName(buffer + sizeof(englishNameSize), buffer + sizeof(englishNameSize) + englishNameSize);
					buffer = buffer + sizeof(englishNameSize) + englishNameSize;

					// �R�����g
					unsigned int commentSize = 0;
					std::memcpy(&commentSize, buffer, sizeof(commentSize));
					std::string comment(buffer + sizeof(commentSize), buffer + sizeof(commentSize) + commentSize);
					buffer = buffer + sizeof(commentSize) + commentSize;

					// �p��̃R�����g
					unsigned int englishCommentSize = 0;
					std::memcpy(&englishCommentSize, buffer, sizeof(englishCommentSize));
					std::string englishComment(buffer + sizeof(englishCommentSize), buffer + sizeof(englishCommentSize) + englishCommentSize);
					buffer = buffer + sizeof(englishCommentSize) + englishCommentSize;
				}

				// ���_�f�[�^�ǂݍ���
				{
					vertexCount = 0;
					std::memcpy(&vertexCount, buffer, sizeof(vertexCount));
					buffer = buffer + sizeof(vertexCount);

					unsigned int addUvCount = header->globals[1];
					unsigned int boneIndexSize = header->globals[5];
					vertices = new Vertex[vertexCount];
					for (int i = 0; i < vertexCount; ++i)
					{
						// ���W
						std::memcpy(&(vertices[i].position), buffer, sizeof(vertices[i].position));
						buffer = buffer + sizeof(vertices[i].position);

						// �@��
						std::memcpy(&(vertices[i].normal), buffer, sizeof(vertices[i].normal));
						buffer = buffer + sizeof(vertices[i].normal);

						// UV
						std::memcpy(&(vertices[i].uv), buffer, sizeof(vertices[i].uv));
						buffer = buffer + sizeof(vertices[i].uv);

						// �ǉ�UV
						if (0 < addUvCount)
						{
							buffer = buffer + (sizeof(DirectX::XMFLOAT4) * addUvCount);
						}

						// �E�F�C�g�ύX����
						{
							unsigned char weightType = 0;
							std::memcpy(&weightType, buffer, sizeof(weightType));
							buffer = buffer + sizeof(weightType);

							// �E�F�C�g��ʖ��̃f�[�^�ǂݍ���
							// ����͎g�p���Ȃ��̂ŃX�L�b�v����
							// TODO:State or Strategy�p�^�[���쐬
							switch (weightType)
							{
								// BDEF1
							case 0:
								buffer = buffer + boneIndexSize;
								break;
								// BDEF2
							case 1:
								buffer = buffer + (boneIndexSize * 2);
								buffer = buffer + sizeof(float);
								break;
								// BDEF4
							case 2:
								buffer = buffer + (boneIndexSize * 4);
								buffer = buffer + (sizeof(float) * 4);
								break;
							case 3:
								buffer = buffer + (boneIndexSize * 2);
								buffer = buffer + sizeof(float);
								buffer = buffer + (sizeof(DirectX::XMFLOAT3) * 3);
								break;
							default:
								throw "��@�ȃE�F�C�g�ύX�����f�[�^�ł��B";
								break;
							}
						}

						// �G�b�W�{��
						// �g�p���Ȃ��̂ŃX�L�b�v
						buffer = buffer + sizeof(float);
					}
				}

				// �|���S���f�[�^�ǂݍ���
				{
					indexCount = 0;
					std::memcpy(&indexCount, buffer, sizeof(indexCount));
					buffer = buffer + sizeof(indexCount);
				}
			}
		}
	};
};
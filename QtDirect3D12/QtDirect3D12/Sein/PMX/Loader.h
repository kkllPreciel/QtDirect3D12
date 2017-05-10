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
		struct Header;
		struct Vertex;

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

			/**
			 *	@brief	���_�����擾����
			 *	@return	���_��
			 */
			unsigned int GetVertexCount() const;

			/**
			 *	@brief	���_�f�[�^���擾����
			 *	@return	���_�f�[�^�ւ̃|�C���^
			 */
			const void* const GetVertices() const;

			/**
			 *	@brief	���_�̃T�C�Y���擾����
			 *	@return	���_�̃T�C�Y
			 */
			unsigned int GetVertexSize() const;

			/**
			 *	@brief	���_�C���f�b�N�X�����擾����
			 *	@return	���_�C���f�b�N�X��
			 */
			unsigned int GetIndexCount() const;

			/**
			 *	@brief	���_�C���f�b�N�X�f�[�^���擾����
			 *	@return	���_�C���f�b�N�X�f�[�^�ւ̃|�C���^
			 */
			const void* const GetIndices() const;

			/**
			 *	@brief	���_�C���f�b�N�X�̃T�C�Y���擾����
			 *	@return	���_�C���f�b�N�X�̃T�C�Y
			 */
			const unsigned int GetIndexSize() const;

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
			char*			buffer;			///< �o�b�t�@
			unsigned int	size;			///< �t�@�C���T�C�Y
			Header*			header;			///< �w�b�_
			unsigned int	vertexCount;	///< ���_��
			Vertex*			vertices;		///< ���_�f�[�^���X�g
			unsigned int	indexCount;		///< ���_�C���f�b�N�X��
			unsigned int*	indices;		///< ���_�C���f�b�N�X�f�[�^���X�g
			unsigned int	polygonCount;	///< �|���S����
		};
	};
};
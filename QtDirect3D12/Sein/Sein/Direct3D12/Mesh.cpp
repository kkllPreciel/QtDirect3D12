/**
 *	@file		Mesh.cpp
 *	@brief		���b�V���Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/07/16
 *	@version	1.0
 */

// include
#include <cstring>
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Sein
{
	namespace Direct3D12
	{
		/**
		 *	@brief	�R���X�g���N�^
		 */
		Mesh::Mesh() : vertexBuffer(nullptr), indexBuffer(nullptr)
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
		 */
		Mesh::~Mesh()
		{

		}

		/**
		 *	@brief	���_�o�b�t�@���擾����
		 *	@return	���_�o�b�t�@�ւ̎Q��
		 */
		const VertexBuffer& Mesh::GetVertexBuffer() const
		{
			return *vertexBuffer;
		}

		/**
		 *	@brief	�C���f�b�N�X�o�b�t�@���擾����
		 *	@return	�C���f�b�N�X�o�b�t�@�ւ̎Q��
		 */
		const IndexBuffer& Mesh::GetIndexBuffer() const
		{
			return *indexBuffer;
		}
	};
};
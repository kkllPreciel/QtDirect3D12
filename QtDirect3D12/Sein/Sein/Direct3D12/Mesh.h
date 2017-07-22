/**
 *	@file		Mesh.h
 *	@brief		���b�V���Ɋւ���w�b�_�t�@�C��
 *	@author		kkllPreciel
 *	@date		2017/07/16
 *	@version	1.0
 */

#pragma once

// include
#include <memory>
#include <d3d12.h>

namespace Sein
{
	namespace Direct3D12
	{
		class VertexBuffer;
		class IndexBuffer;

		/**
		 *	@brief	���b�V���N���X
		 */
		class Mesh
		{
		public:
			/**
			 *	@brief	�R���X�g���N�^
			 */
			Mesh();

			/**
			 *	@brief	�f�X�g���N�^
			 */
			~Mesh();

			/**
			 *	@brief	���_�o�b�t�@���擾����
			 *	@return	���_�o�b�t�@�ւ̎Q��
			 */
			const VertexBuffer& GetVertexBuffer() const;

			/**
			 *	@brief	�C���f�b�N�X�o�b�t�@���擾����
			 *	@return	�C���f�b�N�X�o�b�t�@�ւ̎Q��
			 */
			const IndexBuffer& GetIndexBuffer() const;

		private:
			/**
			 *	@brief	�R�s�[�R���X�g���N�^
			 *	@param	other:�R�s�[���̃C���X�^���X
			 */
			Mesh(const Mesh& other) = delete;

			/**
			 *	@brief	������Z�q�I�y���[�^
			 *	@param	other:������̃C���X�^���X
			 *	@return	�����̃C���X�^���X
			 */
			Mesh& operator = (const Mesh& other) = delete;

		private:
			std::unique_ptr<VertexBuffer>	vertexBuffer;	///< ���_�o�b�t�@
			std::unique_ptr<IndexBuffer>	indexBuffer;	///< ���_�C���f�b�N�X�o�b�t�@
		};
	};
};
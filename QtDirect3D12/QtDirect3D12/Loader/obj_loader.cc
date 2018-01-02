/**
 *  @file     obj_loader.cc
 *  @brief    OBJ�t�@�C���ǂݍ��݂Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/01
 *  @version  1.0
 */

 // include
#include "obj_loader.h"

namespace App
{
  namespace Loader
  {
    namespace Obj
    {
      /**
       *  @brief  OBJ�t�@�C�����f���p�N���X
       */
      class Model final : public IModel
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        Model() : vertex_count_(0), polygon_count_(0), index_count_(0)
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Model() override
        {

        }

        /**
         *  @brief  �R�s�[�R���X�g���N�^
         *  @param  other:�R�s�[���̃C���X�^���X
         */
        Model(const Model& other) = delete;

        /**
         *  @brief  �R�s�[������Z�q�I�y���[�^
         *  @param  other:�R�s�[������̃C���X�^���X
         *  @return �R�s�[�����̃C���X�^���X
         */
        Model& operator = (const Model& other) = delete;

        /**
         *  @brief  ���[�u�R���X�g���N�^
         *  @param  other:���[�u���̃C���X�^���X
         */
        Model(Model&& other) = delete;

        /**
         *  @brief  ���[�u������Z�q�I�y���[�^
         *  @param  other:���[�u������̃C���X�^���X
         *  @return ���[�u�����̃C���X�^���X
         */
        Model& operator = (Model&& other) = delete;

        /**
         *  @brief  ���_�����擾����
         *  @return ���_��
         */
        uint32_t GetVertexCount() override
        {
          return vertex_count_;
        }

        /**
         *  @brief  ���_����ݒ肷��
         *  @param  vertex_count:���_��
         */
        void SetVertexCount(uint32_t vertex_count)
        {
          vertex_count_ = vertex_count;
        }

        /**
         *  @brief  �|���S�������擾����
         *  @return �|���S����
         */
        uint32_t GetPolygonCount() override
        {
          return polygon_count_;
        }

        /**
         *  @brief  �|���S������ݒ肷��
         *  @param  polygon_count:�|���S����
         */
        void SetPolygonCount(uint32_t polygon_count)
        {
          polygon_count_ = polygon_count;
        }

        /**
         *  @brief  ���_�C���f�b�N�X�����擾����
         *  @return ���_�C���f�b�N�X��
         */
        uint32_t GetIndexCount() override
        {
          return index_count_;
        }

        /**
         *  @brief  ���_�C���f�b�N�X����ݒ肷��
         *  @param  index_count:���_�C���f�b�N�X��
         */
        void SetIndexCount(uint32_t index_count)
        {
          index_count_ = index_count;
        }

        /**
         *  @brief  �f�[�^���J������
         */
        void Release() override
        {

        }

      private:
        uint32_t vertex_count_;
        uint32_t polygon_count_;
        uint32_t index_count_;
      };

      /**
       *  @brief  OBJ�t�@�C���ǂݍ��ݗp�N���X
       */
      class Loader final : public ILoader
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        Loader()
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Loader() override
        {

        }

        /**
         *  @brief  �R�s�[�R���X�g���N�^
         *  @param  other:�R�s�[���̃C���X�^���X
         */
        Loader(const Loader& other) = delete;

        /**
         *  @brief  �R�s�[������Z�q�I�y���[�^
         *  @param  other:�R�s�[������̃C���X�^���X
         *  @return �R�s�[�����̃C���X�^���X
         */
        Loader& operator = (const Loader& other) = delete;

        /**
         *  @brief  ���[�u�R���X�g���N�^
         *  @param  other:���[�u���̃C���X�^���X
         */
        Loader(Loader&& other) = delete;

        /**
         *  @brief  ���[�u������Z�q�I�y���[�^
         *  @param  other:���[�u������̃C���X�^���X
         *  @return ���[�u�����̃C���X�^���X
         */
        Loader& operator = (Loader&& other) = delete;

        /**
         *  @brief  �ǂݍ��݂��s��
         *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
         *  @param  status_code:�ǂݍ��݌��ʂ̃X�e�[�^�X�R�[�h
         *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
         */
        std::unique_ptr<IModel> Load(const std::string& file_path, StatusCode* const status_code) override
        {
          std::unique_ptr<Model> model = nullptr;
          if (nullptr != status_code)
          {
            *status_code = StatusCode::UNSUPPORTED;
          }

          // �g���q�`�F�b�N

          // �t�@�C�����݃`�F�b�N

          // �ǂݍ��ݎ��s
          model = std::make_unique<Model>();

          return model;
        }
      };

      /**
       *  @brief  OBJ�t�@�C���ǂݍ��ݗp�C���X�^���X�𐶐�����
       *  @return �ǂݍ��ݗp�C���^�[�t�F�C�X
       */
      std::unique_ptr<ILoader> CreateLoader()
      {
        return std::make_unique<Loader>();
      }
    };
  };
};
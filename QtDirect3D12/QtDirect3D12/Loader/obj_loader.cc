/**
 *  @file     obj_loader.cc
 *  @brief    OBJ�t�@�C���ǂݍ��݂Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/01
 *  @version  1.0
 */

 // include
#include "obj_loader.h"
#include <fstream>
#include <sstream>
#include <map>

namespace App
{
  namespace Loader
  {
    namespace Obj
    {
      namespace
      {
        enum ObjKeywords {
          kIllegal = 0,
          kComment,
          kMaterialFileName,
          kGroup,
          kUseMaterial,
          kVertex,
          kVertexTexture,
          kVertexNormal,
          kPolygon,
        };

        ObjKeywords GetID(std::string keyword)
        {
          std::map<std::string, ObjKeywords> map = {
            { "#", ObjKeywords::kComment },
            { "mtllib", ObjKeywords::kMaterialFileName },
            { "g", ObjKeywords::kGroup },
            { "usemtl", ObjKeywords::kUseMaterial },
            { "v", ObjKeywords::kVertex },
            { "vt", ObjKeywords::kVertexTexture },
            { "vn", ObjKeywords::kVertexNormal },
            { "f", ObjKeywords::kPolygon },
          };

          if (map.count(keyword) == 0)
          {
            return ObjKeywords::kIllegal;
          }

          return map.at(keyword);
        }
      };

      /**
       *  @brief  OBJ�t�@�C�����f���p�N���X
       */
      class Model final : public IModel
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        Model() : vertices_(0), polygon_count_(0), indices_(0)
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Model() override
        {
          Release();
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
         *  @brief  ���_���W�f�[�^���擾����
         *  @return ���_���W�f�[�^�̃��X�g
         */
        const std::vector<DirectX::XMFLOAT4A>& GetVertices() const override
        {
          return vertices_;
        }

        /**
         *  @brief  ���_�����擾����
         *  @return ���_��
         */
        std::size_t GetVertexCount() const override
        {
          return vertices_.size();
        }

        /**
         *  @brief  �|���S�������擾����
         *  @return �|���S����
         */
        std::size_t GetPolygonCount() const override
        {
          return polygon_count_;
        }

        /**
         *  @brief  ���_�C���f�b�N�X�f�[�^���擾����
         *  @return ���_�C���f�b�N�X�f�[�^�̃��X�g
         */
        const std::vector<uint32_t>& GetIndices() const override
        {
          return indices_;
        }

        /**
         *  @brief  ���_�C���f�b�N�X�����擾����
         *  @return ���_�C���f�b�N�X��
         */
        std::size_t GetIndexCount() const override
        {
          return indices_.size();
        }

        /**
         *  @brief  �f�[�^���J������
         */
        void Release() override
        {
          vertices_.clear();
          polygon_count_ = 0;
          indices_.clear();
        }

        /**
         *  @brief  ���_���W��ǉ�����
         *  @param  vertex:�ǉ����钸�_���W
         */
        void AddVertex(const DirectX::XMFLOAT4A& vertex)
        {
          vertices_.emplace_back(vertex);
        }

        /**
         *  @brief  �|���S����ǉ�����
         */
        void AddPolygon()
        {
          ++polygon_count_;
        }

        /**
         *  @brief  ���_�C���f�b�N�X��ǉ�����
         *  @param  polygon_indices:�ǉ����钸�_�C���f�b�N�X�̃|���S���P�ʂł̃��X�g
         */
        void AddIndex(const std::vector<uint32_t>& polygon_indices)
        {
          for (auto index : polygon_indices)
          {
            indices_.emplace_back(index);
          }
        }

      private:
        std::vector<DirectX::XMFLOAT4A> vertices_;  ///< ���_���W���X�g
        std::size_t polygon_count_;                 ///< �|���S����
        std::vector<uint32_t> indices_;             ///< ���_�C���f�b�N�X���X�g
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


          if (nullptr != status_code)
          {
            *status_code = StatusCode::FILE_NOT_FOUND;
          }

          std::ifstream stream;
          stream.open(file_path, std::ifstream::in | std::ifstream::binary);
          if (false == stream.operator bool())
          {
            return model;
          }

          // �ǂݍ��ݎ��s
          std::istreambuf_iterator<char> it(stream);
          std::istreambuf_iterator<char> last;
          std::string data(it, last);
          stream.close();

          // ���f���f�[�^�ǂݍ���(�s�P�ʂŏ������s��)
          std::stringstream ss(data);
          std::string datum;
          while (std::getline(ss, datum, '\n'))
          {
            // �X�y�[�X�܂ł̕��������擾
            auto position = datum.find(' ');

            // �X�y�[�X�����݂��Ȃ�(�Ή����Ȃ�)
            if (std::string::npos == position)
            {
              continue;
            }

            // �L�[���[�h���擾���Ή����鏈�����s��
            switch (GetID(datum.substr(0, position)))
            {
              // �R�����g
            case ObjKeywords::kComment:
              continue;
              break;
              // �}�e���A���t�@�C����
            case ObjKeywords::kMaterialFileName:
              continue;
              break;
              // �O���[�v
            case ObjKeywords::kGroup:
              continue;
              break;
              // ���݂̃O���[�v�f�[�^���g�p����}�e���A����
            case ObjKeywords::kUseMaterial:
              break;
              // ���_���W
            case ObjKeywords::kVertex:
              break;
              // �e�N�X�`�����W
            case ObjKeywords::kVertexTexture:
              break;
              // ���_�@���x�N�g��
            case ObjKeywords::kVertexNormal:
              break;
              // �|���S���f�[�^
            case ObjKeywords::kPolygon:
              break;
              // �s���L�[���[�h
            default:
              continue;
              break;
            }
          }

          // �_�~�[�f�[�^���쐬���Ԃ�
          model = std::make_unique<Model>();
          model->AddVertex(DirectX::XMFLOAT4A(1.0f, -1.0f, -1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(1.0f, -1.0f, 1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(-1.0f, -1.0f, 1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(-1.0f, -1.0f, -1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(1.0f, 1.0f, -1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(1.0f, 1.0f, 1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(-1.0f, 1.0f, 1.0f, 0.0f));
          model->AddVertex(DirectX::XMFLOAT4A(-1.0f, 1.0f, -1.0f, 0.0f));

          for (auto i = 0; i < 12; ++i)
            model->AddPolygon();

          model->AddIndex(std::vector<uint32_t> {
            1, 2, 3,
            1, 3, 4,
            5, 8, 7,
            5, 7, 6,
            1, 5, 6,
            1, 6, 2,
            2, 6, 7,
            2, 7, 3,
            3, 7, 8,
            3, 8, 4,
            5, 1, 4,
            5, 4, 8,
          });

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
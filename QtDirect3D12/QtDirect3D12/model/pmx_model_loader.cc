/**
 *  @file     pmx_model_loader.cc
 *  @brief    PMX�t�@�C���ǂݍ��݂Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/02/17
 *  @version  1.0
 */

 // include
#include "model/model.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>

namespace App
{
  namespace
  {
    // �A���C�������g��1�o�C�g�ɐݒ�
#pragma pack(push, 1)
    /**
     *  @brief  Pmx�w�b�_�f�[�^�\����
     */
    struct Header
    {
      char  signature[4]; ///< �V�O�l�`��
      float version;      ///< �o�[�W����
      char  globalsCount; ///< �t�@�C���S�̂ł̐ݒ���̌�(�G���R�[�h����, �ǉ�UV����)
      char  globals[8];   ///< �t�@�C���S�̂ł̐ݒ���
    };
    // �A���C�������g���f�t�H���g�̐ݒ�ɖ߂�
#pragma pack(pop)

    /**
     *  @brief  PMX�t�@�C�����f���p�N���X
     */
    class Model final : public IModel
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      Model() : polygon_count_(0), points_(0), normals_(0), texture_coords_(0),
        point_indices_(0), normal_indices_(0), texture_coord_indices_(0)
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
       *  @brief  �|���S�������擾����
       *  @return �|���S����
       */
      uint32_t GetPolygonCount() const override
      {
        return polygon_count_;
      }
    
      /**
       *  @brief  ���_���W���X�g���擾����
       *  @return ���_���W�̃��X�g
       */
      const std::vector<DirectX::XMFLOAT3>& GetControlPoints() const override
      {
        return points_;
      }
    
      /**
       *  @brief  ���_���W�����擾����
       *  @return ���_���W��
       */
      uint32_t GetControlPointCount() const override
      {
        return static_cast<uint32_t>(points_.size());
      }
    
      /**
       *  @brief  ���_�@���x�N�g�����X�g���擾����
       *  @return ���_�@���x�N�g���̃��X�g
       */
      const std::vector<DirectX::XMFLOAT3>& GetNormals() const override
      {
        return normals_;
      }
    
      /**
       *  @brief  ���_�@���x�N�g�������擾����
       *  @return ���_�@���x�N�g����
       */
      uint32_t GetNormalCount() const override
      {
        return static_cast<uint32_t>(normals_.size());
      }
    
      /**
       *  @brief  ���_�e�N�X�`�����W���X�g���擾����
       *  @return ���_�e�N�X�`�����W�̃��X�g
       */
      const std::vector<DirectX::XMFLOAT2>& GetTextureCoords() const override
      {
        return texture_coords_;
      }
    
      /**
       *  @brief  �e�N�X�`�����W�����擾����
       *  @return �e�N�X�`�����W��
       */
      uint32_t GetTextureCoordCount() const override
      {
        return static_cast<uint32_t>(texture_coords_.size());
      }

      /**
       *  @brief  ���_���W�C���f�b�N�X�f�[�^���擾����
       *  @return ���_���W�C���f�b�N�X�f�[�^�̃��X�g
       */
      const std::vector<uint32_t>& GetControlPointIndices() const override
      {
        return point_indices_;
      }
      
      /**
       *  @brief  ���_���W�C���f�b�N�X�����擾����
       *  @return ���_���W�C���f�b�N�X��
       */
      uint32_t GetControlPointIndexCount() const override
      {
        return static_cast<uint32_t>(point_indices_.size());
      }
    
      /**
       *  @brief  ���_�@���x�N�g���C���f�b�N�X���X�g���擾����
       *  @return ���_�@���x�N�g���C���f�b�N�X�̃��X�g
       */
      const std::vector<uint32_t>& GetNormalIndices() const override
      {
        return normal_indices_;
      }
    
      /**
       *  @brief  ���_�@���x�N�g���C���f�b�N�X�����擾����
       *  @return ���_�@���x�N�g���C���f�b�N�X��
       */
      uint32_t GetNormalIndexCount() const override
      {
        return static_cast<uint32_t>(normal_indices_.size());
      }
    
      /**
       *  @brief  ���_�e�N�X�`�����W�C���f�b�N�X���X�g���擾����
       *  @return ���_�e�N�X�`�����W�C���f�b�N�X�̃��X�g
       */
      const std::vector<uint32_t>& GetTextureCoordIndices() const override
      {
        return texture_coord_indices_;
      }
    
      /**
       *  @brief  ���_�e�N�X�`�����W�C���f�b�N�X�����擾����
       *  @return ���_�e�N�X�`�����W�C���f�b�N�X��
       */
      uint32_t GetTextureCoordIndexCount() const override
      {
        return static_cast<uint32_t>(texture_coord_indices_.size());
      }

      /**
       *  @brief  �w�b�_�f�[�^���擾����
       *  @return �w�b�_�f�[�^
       */
      const Header& GetHeader() const
      {
        return *header_;
      }
    
      /**
       *  @brief  �f�[�^���J������
       */
      void Release() override
      {
        polygon_count_ = 0;
        points_.clear();
        normals_.clear();
        texture_coords_.clear();
        point_indices_.clear();
        normal_indices_.clear();
        texture_coord_indices_.clear();

        header_.reset();
      }

      /**
       *  @brief  �|���S����ǉ�����
       */
      void AddPolygon()
      {
        ++polygon_count_;
      }
    
      /**
       *  @brief  ���W��ǉ�����
       *  @param  point:�ǉ�������W
       */
      void AddControlPoint(const DirectX::XMFLOAT3& point)
      {
        points_.emplace_back(point);
      }
    
      /**
       *  @brief  �@���x�N�g����ǉ�����
       *  @param  normal:�ǉ�����@���x�N�g��
       */
      void AddNormal(const DirectX::XMFLOAT3& normal)
      {
        normals_.emplace_back(normal);
      }
    
      /**
       *  @brief  �e�N�X�`�����W��ǉ�����
       *  @param  texture_coord:�ǉ�����e�N�X�`�����W
       */
      void AddTextureCoord(const DirectX::XMFLOAT2& texture_coord)
      {
        texture_coords_.emplace_back(texture_coord);
      }
    
      /**
       *  @brief  ���_���W�C���f�b�N�X��ǉ�����
       *  @param  index:�ǉ����钸�_���W�C���f�b�N�X
       */
      void AddPointIndex(const uint32_t index)
      {
        point_indices_.emplace_back(index);
      }
    
      /**
       *  @brief  ���_�@���x�N�g���C���f�b�N�X��ǉ�����
       *  @param  index:�ǉ����钸�_�@���x�N�g���C���f�b�N�X
       */
      void AddNormalIndex(const uint32_t index)
      {
        normal_indices_.emplace_back(index);
      }
    
      /**
       *  @brief  ���_�e�N�X�`�����W�C���f�b�N�X��ǉ�����
       *  @param  index:�ǉ����钸�_�e�N�X�`�����W�C���f�b�N�X
       */
      void AddTextureCoordIndex(const uint32_t index)
      {
        texture_coord_indices_.emplace_back(index);
      }

      /**
       *  @brief  �w�b�_�f�[�^��ݒ肷��
       *  @param  header:�w�b�_�f�[�^
       */
      void AddHeader(Header* header)
      {
        header_.reset(header);
      }
    
    private:
      uint32_t polygon_count_;                        ///< �|���S����
      std::vector<DirectX::XMFLOAT3> points_;         ///< ���W���X�g
      std::vector<DirectX::XMFLOAT3> normals_;        ///< �@���x�N�g�����X�g
      std::vector<DirectX::XMFLOAT2> texture_coords_; ///< �e�N�X�`�����W���X�g
      std::vector<uint32_t> point_indices_;           ///< ���_���W�C���f�b�N�X���X�g
      std::vector<uint32_t> normal_indices_;          ///< ���_�@���x�N�g���C���f�b�N�X���X�g
      std::vector<uint32_t> texture_coord_indices_;   ///< ���_�e�N�X�`�����W�C���f�b�N�X���X�g

      std::unique_ptr<Header> header_;                ///< �w�b�_�f�[�^
    };
  };

  /**
   *  @brief  PMX�t�H�[�}�b�g�t�@�C�����烂�f���f�[�^���쐬����
   *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
   *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
   */
  std::unique_ptr<IModel> IModel::CreateFromPmx(const std::string& file_path)
  {
    std::unique_ptr<Model> model = nullptr;
    std::unique_ptr<char[]> buffer = nullptr;
    std::uint32_t size = 0;

    // �t�@�C�����e�̓ǂݍ���
    {
      namespace fs = std::experimental::filesystem;

      // �t�@�C�������݂��Ȃ�
      if (false == fs::exists(file_path))
      {
        return model;
      }

      std::ifstream ifs(file_path, std::ios::in | std::ios::binary);

      ifs.seekg(0, std::fstream::end);
      unsigned int end = ifs.tellg();

      // �擪�ɖ߂邽�߂Ɉ�xclear()��������B
      // ��������Ȃ��Ǝ���seekg()�ŃR�P��Ƃ�������B
      ifs.clear();

      ifs.seekg(0, std::fstream::beg);
      size = end - ifs.tellg();

      // �o�b�t�@�̊m��
      buffer = std::make_unique<char[]>(size + 1);

      ifs.read(buffer.get(), size);
      ifs.close();
    }

    // ���f���f�[�^�ǂݍ���
    {
      auto temporary = buffer.get();

      // �w�b�_�ǂݍ���
      {
        // �t�@�C���T�C�Y���w�b�_�T�C�Y����
        if (size < sizeof(Header))
        {
          return model;
        }

        model = std::make_unique<Model>();

        auto header = new Header;
        std::memcpy(header, temporary, sizeof(Header));
        model->AddHeader(header);
        temporary = temporary + sizeof(Header);

        // ��@�f�[�^�`�F�b�N
        // �V�O�l�`�����uPMX �v�łȂ�
        // �o�[�W�������u2.0�v���u2.1�v�łȂ�
      }

      // ���f�����ǂݍ���
      {
        // ���f����
        unsigned int nameSize = 0;
        std::memcpy(&nameSize, temporary, sizeof(nameSize));
        std::string name(temporary + sizeof(nameSize), temporary + sizeof(nameSize) + nameSize);
        temporary = temporary + sizeof(nameSize) + nameSize;

        // �p��̃��f����
        unsigned int englishNameSize = 0;
        std::memcpy(&englishNameSize, temporary, sizeof(englishNameSize));
        std::string englishName(temporary + sizeof(englishNameSize), temporary + sizeof(englishNameSize) + englishNameSize);
        temporary = temporary + sizeof(englishNameSize) + englishNameSize;

        // �R�����g
        unsigned int commentSize = 0;
        std::memcpy(&commentSize, temporary, sizeof(commentSize));
        std::string comment(temporary + sizeof(commentSize), temporary + sizeof(commentSize) + commentSize);
        temporary = temporary + sizeof(commentSize) + commentSize;

        // �p��̃R�����g
        unsigned int englishCommentSize = 0;
        std::memcpy(&englishCommentSize, temporary, sizeof(englishCommentSize));
        std::string englishComment(temporary + sizeof(englishCommentSize), temporary + sizeof(englishCommentSize) + englishCommentSize);
        temporary = temporary + sizeof(englishCommentSize) + englishCommentSize;
      }

      // ���_�f�[�^�ǂݍ���
      {
        auto vertex_count = 0;
        std::memcpy(&vertex_count, temporary, sizeof(4));
        temporary = temporary + sizeof(4);

        unsigned int addUvCount = model->GetHeader().globals[1];
        unsigned int boneIndexSize = model->GetHeader().globals[5];

        DirectX::XMFLOAT3 point;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT2 texture_coord;

        for (auto i = 0; i < vertex_count; ++i)
        {
          // ���W
          std::memcpy(&point, temporary, sizeof(DirectX::XMFLOAT3));
          temporary = temporary + sizeof(DirectX::XMFLOAT3);

          // �@��
          std::memcpy(&normal, temporary, sizeof(DirectX::XMFLOAT3));
          temporary = temporary + sizeof(DirectX::XMFLOAT3);

          // UV
          std::memcpy(&texture_coord, temporary, sizeof(DirectX::XMFLOAT2));
          temporary = temporary + sizeof(DirectX::XMFLOAT2);

          // �ǉ�UV
          if (0 < addUvCount)
          {
            temporary = temporary + (sizeof(DirectX::XMFLOAT4) * addUvCount);
          }

          // �E�F�C�g�ύX����
          {
            unsigned char weightType = 0;
            std::memcpy(&weightType, temporary, sizeof(weightType));
            temporary = temporary + sizeof(weightType);

            // �E�F�C�g��ʖ��̃f�[�^�ǂݍ���
            // ����͎g�p���Ȃ��̂ŃX�L�b�v����
            // TODO:State or Strategy�p�^�[���쐬
            switch (weightType)
            {
              // BDEF1
            case 0:
              temporary = temporary + boneIndexSize;
              break;
              // BDEF2
            case 1:
              temporary = temporary + (boneIndexSize * 2);
              temporary = temporary + sizeof(float);
              break;
              // BDEF4
            case 2:
              temporary = temporary + (boneIndexSize * 4);
              temporary = temporary + (sizeof(float) * 4);
              break;
            case 3:
              temporary = temporary + (boneIndexSize * 2);
              temporary = temporary + sizeof(float);
              temporary = temporary + (sizeof(DirectX::XMFLOAT3) * 3);
              break;
            default:
              throw "��@�ȃE�F�C�g�ύX�����f�[�^�ł��B";
              break;
            }
          }

          // �G�b�W�{��
          // �g�p���Ȃ��̂ŃX�L�b�v
          temporary = temporary + sizeof(float);

          model->AddControlPoint(point);
          model->AddNormal(normal);
          model->AddTextureCoord(texture_coord);
        }
      }

      // ���_�C���f�b�N�X�f�[�^�ǂݍ���
      {
        auto index_count_ = 0;
        std::memcpy(&index_count_, temporary, sizeof(4));
        temporary = temporary + sizeof(4);

        unsigned int vertexIndexSize = model->GetHeader().globals[2];
        std::vector<std::uint32_t> indices(index_count_);

        std::memcpy(indices.data(), temporary, vertexIndexSize * index_count_);
        temporary = temporary + (vertexIndexSize * index_count_);

        for (auto index : indices)
        {
          model->AddPointIndex(index);
          model->AddNormalIndex(index);
          model->AddTextureCoordIndex(index);
        }

        auto polygon_count = index_count_ / 3;
        for (auto i = 0; i < polygon_count; ++i)
        {
          model->AddPolygon();
        }
      }

      // �e�N�X�`���ǂݍ���
      {
        unsigned int textureCount = 0;
        std::memcpy(&textureCount, temporary, sizeof(textureCount));
        temporary = temporary + sizeof(textureCount);

        for (unsigned int i = 0; i < textureCount; ++i)
        {
          // �e�N�X�`���p�X�T�C�Y
          unsigned int texturePathSize = 0;
          std::memcpy(&texturePathSize, temporary, sizeof(texturePathSize));
          temporary = temporary + sizeof(texturePathSize);

          // �e�N�X�`��
          temporary = temporary + texturePathSize;
        }
      }

      // �}�e���A���ǂݍ���
      {
        unsigned int materialCount = 0;
        std::memcpy(&materialCount, temporary, sizeof(materialCount));
        temporary = temporary + sizeof(materialCount);

        for (unsigned int i = 0; i < materialCount; ++i)
        {
          // �}�e���A����
          unsigned int nameSize = 0;
          std::memcpy(&nameSize, temporary, sizeof(nameSize));
          std::string name(temporary + sizeof(nameSize), temporary + sizeof(nameSize) + nameSize);
          temporary = temporary + sizeof(nameSize) + nameSize;

          // �}�e���A����(�p��)
          unsigned int englishNameSize = 0;
          std::memcpy(&englishNameSize, temporary, sizeof(englishNameSize));
          std::string englishName(temporary + sizeof(englishNameSize), temporary + sizeof(englishNameSize) + englishNameSize);
          temporary = temporary + sizeof(englishNameSize) + englishNameSize;

          // �g�U���ˌ�(Diffuse)
          temporary = temporary + sizeof(DirectX::XMFLOAT4);

          // ���ʔ��ˌ�(Specular)
          temporary = temporary + sizeof(DirectX::XMFLOAT3);

          // ���ʔ��˂̋��x
          temporary = temporary + sizeof(float);

          // ����(Ambient)
          temporary = temporary + sizeof(DirectX::XMFLOAT3);

          // �`��t���O
          temporary = temporary + sizeof(unsigned char);

          // �G�b�W�F
          temporary = temporary + sizeof(DirectX::XMFLOAT4);

          // �G�b�W�T�C�Y
          temporary = temporary + sizeof(float);

          // �ʏ�e�N�X�`��
          temporary = temporary + model->GetHeader().globals[3];

          // �X�t�B�A�e�N�X�`��
          temporary = temporary + model->GetHeader().globals[3];

          // �X�t�B�A���[�h
          temporary = temporary + sizeof(unsigned char);

          // ���LToon�t���O
          bool is_share = false;
          std::memcpy(&is_share, temporary, sizeof(is_share));
          temporary = temporary + sizeof(unsigned char);

          temporary = temporary + (is_share ? sizeof(unsigned char) : model->GetHeader().globals[3]);

          // ����
          unsigned int memoSize = 0;
          std::memcpy(&memoSize, temporary, sizeof(memoSize));
          std::string memoName(temporary + sizeof(memoSize), temporary + sizeof(memoSize) + memoSize);
          temporary = temporary + sizeof(memoSize) + memoSize;

          // �}�e���A���ɑΉ����钸�_�C���f�b�N�X��
          unsigned int vetexCount = 0;
          std::memcpy(&vetexCount, temporary, sizeof(vetexCount));
          temporary = temporary + sizeof(vetexCount);
        }
      }
    }

    return model;
  }
};
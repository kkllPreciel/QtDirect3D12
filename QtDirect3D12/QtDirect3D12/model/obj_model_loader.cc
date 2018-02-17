/**
 *  @file     obj_model_loader.cc
 *  @brief    OBJ�t�@�C���ǂݍ��݂Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/01/01
 *  @version  1.0
 */

 // include
#include "model/model.h"
#include <fstream>
#include <sstream>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>

namespace App
{
  namespace
  {
    enum ObjKeywords {
      kIllegal = 0,
      kComment,
      kMaterialFileName,
      kGroup,
      kUseMaterial,
      kControlPoint,
      kVertexTexture,
      kVertexNormal,
      kPolygon,
    };

    ObjKeywords GetID(std::string keyword)
    {
      std::map<std::string, ObjKeywords> map =
      {
        { "#", ObjKeywords::kComment },
        { "mtllib", ObjKeywords::kMaterialFileName },
        { "g", ObjKeywords::kGroup },
        { "usemtl", ObjKeywords::kUseMaterial },
        { "v", ObjKeywords::kControlPoint },
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

    /**
     *  @brief  OBJ�t�@�C�����f���p�N���X
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
       *  @param  polygon_indices:�ǉ����钸�_���W�C���f�b�N�X�̃|���S���P�ʂł̃��X�g
       */
      void AddPointIndex(const std::vector<uint32_t>& polygon_indices)
      {
        for (auto index : polygon_indices)
        {
          point_indices_.emplace_back(index);
        }
      }
    
      /**
       *  @brief  ���_�@���x�N�g���C���f�b�N�X��ǉ�����
       *  @param  polygon_indices:���_�@���x�N�g���C���f�b�N�X�̃|���S���P�ʂł̃��X�g
       */
      void AddNormalIndex(const std::vector<uint32_t>& polygon_indices)
      {
        for (auto index : polygon_indices)
        {
          normal_indices_.emplace_back(index);
        }
      }
    
      /**
       *  @brief  ���_�e�N�X�`�����W�C���f�b�N�X��ǉ�����
       *  @param  polygon_indices:�ǉ����钸�_�e�N�X�`�����W�C���f�b�N�X�̃|���S���P�ʂł̃��X�g
       */
      void AddTextureCoordIndex(const std::vector<uint32_t>& polygon_indices)
      {
        for (auto index : polygon_indices)
        {
          texture_coord_indices_.emplace_back(index);
        }
      }
    
    private:
      uint32_t polygon_count_;                        ///< �|���S����
      std::vector<DirectX::XMFLOAT3> points_;         ///< ���W���X�g
      std::vector<DirectX::XMFLOAT3> normals_;        ///< �@���x�N�g�����X�g
      std::vector<DirectX::XMFLOAT2> texture_coords_; ///< �e�N�X�`�����W���X�g
      std::vector<uint32_t> point_indices_;           ///< ���_���W�C���f�b�N�X���X�g
      std::vector<uint32_t> normal_indices_;          ///< ���_�@���x�N�g���C���f�b�N�X���X�g
      std::vector<uint32_t> texture_coord_indices_;   ///< ���_�e�N�X�`�����W�C���f�b�N�X���X�g
    };
  };

  /**
   *  @brief  OBJ�t�H�[�}�b�g�t�@�C�����烂�f���f�[�^���쐬����
   *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
   *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
   */
  std::unique_ptr<IModel> IModel::CreateFromObj(const std::string& file_path)
  {
    std::unique_ptr<Model> model = nullptr;

    // TODO:�g���q�`�F�b�N

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

    // C4996�x���𖳌��ɂ���
#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(push)
#pragma warning(disable:4996)
#endif
    // �s���ɕ�������
    std::vector<std::string> list;
    boost::split(list, data, boost::is_any_of("\r\n"));
#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(pop)
#endif

    model = std::make_unique<Model>();

    // ���f���f�[�^�ǂݍ���(�s�P�ʂŏ������s��)
    for (auto line : list)
    {
      std::vector<std::string> parts;
      boost::split(parts, line, boost::is_space());

      // �����o���Ȃ�����(��@�ȃf�[�^)
      if (parts.size() <= 1)
      {
        continue;
      }

      // �L�[���[�h���擾���Ή����鏈�����s��
      switch (GetID(parts.at(0)))
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
      case ObjKeywords::kControlPoint:
        // TODO:�v�f����4�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
        model->AddControlPoint(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
        break;
        // �e�N�X�`�����W
      case ObjKeywords::kVertexTexture:
        // TODO:�v�f����3�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
        model->AddTextureCoord(DirectX::XMFLOAT2(std::stof(parts[1]), std::stof(parts[2])));
        break;
        // ���_�@���x�N�g��
      case ObjKeywords::kVertexNormal:
        // TODO:�v�f����4�łȂ��ꍇ�͈�@�f�[�^�Ƃ��Ė߂�
        model->AddNormal(DirectX::XMFLOAT3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
        break;
        // �|���S���f�[�^
      case ObjKeywords::kPolygon:
      {
        std::vector<uint32_t> point_indices;
        std::vector<uint32_t> normal_indices;
        std::vector<uint32_t> texture_coord_indices;
        for (decltype(parts.size()) i = 1; i < parts.size(); ++i)
        {
          std::vector<std::string> indices;
          boost::split(indices, parts[i], boost::is_any_of("/"));
          for (auto index : indices | boost::adaptors::indexed())
          {
            if (false == index.value().empty())
            {
              // ���W�E�e�N�X�`�����W�E�@���x�N�g����0�Ԗڂ���Ȃ̂ŁA-1���s��
              const auto value = std::stoi(index.value()) - 1;
              switch (index.index())
              {
              case 0:

                point_indices.emplace_back(value);
                break;
              case 1:
                texture_coord_indices.emplace_back(value);
                break;
              case 2:
                normal_indices.emplace_back(value);
                break;
              }
            }
          }
        }
        model->AddPointIndex(point_indices);
        model->AddTextureCoordIndex(texture_coord_indices);
        model->AddNormalIndex(normal_indices);
        model->AddPolygon();
      }
      break;
      // �s���L�[���[�h
      default:
        continue;
        break;
      }
    }

    return model;
  }
};
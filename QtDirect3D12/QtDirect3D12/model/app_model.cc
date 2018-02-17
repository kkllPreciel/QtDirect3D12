/**
 *  @file     app_model.cc
 *  @brief    �A�v���P�[�V�����Ŏg�p���郂�f���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/02/15
 *  @version  1.0
 */

 // include
#include "model/app_model.h"
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>

namespace App
{
  namespace
  {
    /**
     *  @brief  ���f���p�N���X
     */
    class AppModel final : public IAppModel
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      AppModel() : vertices_(0), indices_(0), model_(nullptr), vertex_buffer_(nullptr), index_buffer_(nullptr)
      {
    
      }
    
      /**
       *  @brief  �f�X�g���N�^
       */
      ~AppModel() override
      {
        Release();
      }
    
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      AppModel(const AppModel& other) = delete;
    
      /**
       *  @brief  �R�s�[������Z�q�I�y���[�^
       *  @param  other:�R�s�[������̃C���X�^���X
       *  @return �R�s�[�����̃C���X�^���X
       */
      AppModel& operator = (const AppModel& other) = delete;
    
      /**
       *  @brief  ���[�u�R���X�g���N�^
       *  @param  other:���[�u���̃C���X�^���X
       */
      AppModel(AppModel&& other) = delete;
    
      /**
       *  @brief  ���[�u������Z�q�I�y���[�^
       *  @param  other:���[�u������̃C���X�^���X
       *  @return ���[�u�����̃C���X�^���X
       */
      AppModel& operator = (AppModel&& other) = delete;

      /**
       *  @brief  ���_���X�g���擾����
       *  @return ���_�̃��X�g
       */
      const std::vector<Vertex>& GetVertices() const override
      {
        return vertices_;
      }
      
      /**
       *  @brief  ���_�C���f�b�N�X���X�g���擾����
       *  @return ���_�C���f�b�N�X�̃��X�g
       */
      const std::vector<uint32_t>& GetIndices() const override
      {
        return indices_;
      }

      /**
       *  @brief  ���_�o�b�t�@���擾����
       *  @return ���_�o�b�t�@
       */
      const Sein::Direct3D12::VertexBuffer& GetVertexBuffer() const override
      {
        return *vertex_buffer_;
      }
  
      /**
       *  @brief  �C���f�b�N�X�o�b�t�@���擾����
       *  @return �C���f�b�N�X�o�b�t�@
       */
      const Sein::Direct3D12::IndexBuffer& GetIndexBuffer() const override
      {
        return *index_buffer_;
      }
      
      /**
       *  @brief  �|���S�������擾����
       *  @return �|���S����
       */
      uint32_t GetPolygonCount() const override
      {
        return model_->GetPolygonCount();
      }
    
      /**
       *  @brief  ���_���W���X�g���擾����
       *  @return ���_���W�̃��X�g
       */
      const std::vector<DirectX::XMFLOAT3>& GetControlPoints() const override
      {
        return model_->GetControlPoints();
      }
    
      /**
       *  @brief  ���_���W�����擾����
       *  @return ���_���W��
       */
      uint32_t GetControlPointCount() const override
      {
        return  model_->GetControlPointCount();
      }
    
      /**
       *  @brief  ���_�@���x�N�g�����X�g���擾����
       *  @return ���_�@���x�N�g���̃��X�g
       */
      const std::vector<DirectX::XMFLOAT3>& GetNormals() const override
      {
        return model_->GetNormals();
      }
    
      /**
       *  @brief  ���_�@���x�N�g�������擾����
       *  @return ���_�@���x�N�g����
       */
      uint32_t GetNormalCount() const override
      {
        return model_->GetNormalCount();
      }
    
      /**
       *  @brief  ���_�e�N�X�`�����W���X�g���擾����
       *  @return ���_�e�N�X�`�����W�̃��X�g
       */
      const std::vector<DirectX::XMFLOAT2>& GetTextureCoords() const override
      {
        return model_->GetTextureCoords();
      }
    
      /**
       *  @brief  �e�N�X�`�����W�����擾����
       *  @return �e�N�X�`�����W��
       */
      uint32_t GetTextureCoordCount() const override
      {
        return model_->GetTextureCoordCount();
      }

      /**
       *  @brief  ���_���W�C���f�b�N�X�f�[�^���擾����
       *  @return ���_���W�C���f�b�N�X�f�[�^�̃��X�g
       */
      const std::vector<uint32_t>& GetControlPointIndices() const override
      {
        return model_->GetControlPointIndices();
      }
      
      /**
       *  @brief  ���_���W�C���f�b�N�X�����擾����
       *  @return ���_���W�C���f�b�N�X��
       */
      uint32_t GetControlPointIndexCount() const override
      {
        return model_->GetControlPointIndexCount();
      }
    
      /**
       *  @brief  ���_�@���x�N�g���C���f�b�N�X���X�g���擾����
       *  @return ���_�@���x�N�g���C���f�b�N�X�̃��X�g
       */
      const std::vector<uint32_t>& GetNormalIndices() const override
      {
        return model_->GetNormalIndices();
      }
    
      /**
       *  @brief  ���_�@���x�N�g���C���f�b�N�X�����擾����
       *  @return ���_�@���x�N�g���C���f�b�N�X��
       */
      uint32_t GetNormalIndexCount() const override
      {
        return model_->GetNormalIndexCount();
      }
    
      /**
       *  @brief  ���_�e�N�X�`�����W�C���f�b�N�X���X�g���擾����
       *  @return ���_�e�N�X�`�����W�C���f�b�N�X�̃��X�g
       */
      const std::vector<uint32_t>& GetTextureCoordIndices() const override
      {
        return model_->GetTextureCoordIndices();
      }
    
      /**
       *  @brief  ���_�e�N�X�`�����W�C���f�b�N�X�����擾����
       *  @return ���_�e�N�X�`�����W�C���f�b�N�X��
       */
      uint32_t GetTextureCoordIndexCount() const override
      {
        return model_->GetTextureCoordIndexCount();
      }
    
      /**
       *  @brief  �f�[�^���J������
       */
      void Release() override
      {
        vertices_.clear();
        indices_.clear();

        vertex_buffer_.reset();
        index_buffer_.reset();

        model_->Release();
        model_.reset();
      }

      /**
       *  @brief  ���_��ǉ�����
       *  @param  vertex:�ǉ����钸�_
       */
      void AddVertex(const Vertex& vertex)
      {
        vertices_.emplace_back(vertex);
      }

      /**
       *  @brief  ���_�C���f�b�N�X��ǉ�����
       *  @param  indices:�ǉ����钸�_�C���f�b�N�X�̃|���S���P�ʂł̃��X�g
       */
      void AddIndex(const std::vector<uint32_t>& polygon_indices)
      {
        for (auto index : polygon_indices)
        {
          indices_.emplace_back(index);
        }
      }

      /**
       *  @brief  ���f����ݒ肷��
       *  @param  model:���f��
       */
      void SetModel(App::IModel* model)
      {
        model_.reset(model);
      }

      /**
       *  @brief  ���_�o�b�t�@��ݒ肷��
       *  @param  vertex_buffer:���_�o�b�t�@
       */
      void SetVertexBuffer(Sein::Direct3D12::VertexBuffer* vertex_buffer)
      {
        vertex_buffer_.reset(vertex_buffer);
      }

      /**
       *  @brief  �C���f�b�N�X�o�b�t�@��ݒ肷��
       *  @param  index_buffer:�C���f�b�N�X�o�b�t�@
       */
      void SetIndexBuffer(Sein::Direct3D12::IndexBuffer* index_buffer)
      {
        index_buffer_.reset(index_buffer);
      }
    
    private:
      std::vector<Vertex> vertices_;                                  ///< ���_���X�g
      std::vector<uint32_t> indices_;                                 ///< �C���f�b�N�X���X�g
      std::unique_ptr<App::IModel> model_;                            ///< ���f��
      std::unique_ptr<Sein::Direct3D12::VertexBuffer> vertex_buffer_; ///< ���_�o�b�t�@
      std::unique_ptr<Sein::Direct3D12::IndexBuffer> index_buffer_;   ///< �C���f�b�N�X�o�b�t�@
    };
  };

  /**
   *  @brief  OBJ�t�H�[�}�b�g�t�@�C�����烂�f���f�[�^���쐬����
   *  @param  device:�f�o�C�X
   *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
   *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
   */
  std::unique_ptr<IAppModel> IAppModel::LoadFromObj(const Sein::Direct3D12::Device& device, const std::string& file_path)
  {
    std::unique_ptr<AppModel> app_model = nullptr;

    auto model = IModel::CreateFromObj(file_path);
    if (!model)
    {
      return app_model;
    }

    app_model = std::make_unique<AppModel>();
    app_model->SetModel(model.release());

    // �A�v���P�[�V�����p�̒��_�f�[�^�𐶐�����
    std::vector<Vertex> vertices;
    std::unordered_map<std::string, uint32_t> map;
    std::vector<uint32_t> indices;

    const auto points = app_model->GetControlPoints();
    const auto normals = app_model->GetNormals();
    const auto tex_coords = app_model->GetTextureCoords();
    const auto point_indices = app_model->GetControlPointIndices();
    const auto normal_indices = app_model->GetNormalIndices();
    const auto tex_coords_indices = app_model->GetTextureCoordIndices();

    // ���_�f�[�^���X�g�ƁA���_�f�[�^�C���f�b�N�X�ƃn�b�V���������X�g���쐬����
    for (const auto point : point_indices | boost::adaptors::indexed())
    {
      const auto index = point.index();

      // �n�b�V�����쐬
      std::string hash = std::to_string(point.value());

      if (tex_coords_indices.empty() == false)
      {
        hash += '-' + std::to_string(tex_coords_indices[index]);
      }

      if (normal_indices.empty() == false)
      {
        hash += '-' + std::to_string(normal_indices[index]);
      }

      // �n�b�V�������݂��Ȃ��ꍇ�͒��_�f�[�^��ǉ�����
      if (map.count(hash) == 0)
      {
        // ���_�f�[�^
        Vertex vertex = {};
        vertex.position = points.at(point.value());

        if (normal_indices.empty() == false)
        {
          vertex.normal = normals.at(normal_indices.at(index));
        }

        if (tex_coords_indices.empty() == false)
        {
          vertex.texcoord = tex_coords.at(tex_coords_indices.at(index));
        }

        vertices.emplace_back(vertex);

        // �n�b�V�����X�g���X�V
        map.insert({ hash, static_cast<uint32_t>(vertices.size() - 1) });
      }

      // �C���f�b�N�X���X�g�ɃC���f�b�N�X��ǉ�����
      indices.emplace_back(map.at(hash));
    }

    auto vertex_size = sizeof(App::IAppModel::Vertex);
    auto vertex_buffer = std::make_unique<Sein::Direct3D12::VertexBuffer>();
    auto index_buffer = std::make_unique<Sein::Direct3D12::IndexBuffer>();
    vertex_buffer->Create(&(device.GetDevice()), vertex_size * vertices.size(), vertex_size, &(vertices[0]));
    index_buffer->Create(&(device.GetDevice()), sizeof(uint32_t) * indices.size(), &(indices[0]), DXGI_FORMAT_R32_UINT);

    for (decltype(auto) vertex : vertices)
    {
      app_model->AddVertex(vertex);
    }

    app_model->AddIndex(indices);

    app_model->SetVertexBuffer(vertex_buffer.release());
    app_model->SetIndexBuffer(index_buffer.release());

    return app_model;
  }

  /**
   *  @brief  PMX�t�H�[�}�b�g�t�@�C�����烂�f���f�[�^���쐬����
   *  @param  device:�f�o�C�X
   *  @param  file_path:�ǂݍ��݂��s���t�@�C���̃p�X
   *  @return ���f���C���^�[�t�F�C�X�ւ̃|�C���^
   */
  std::unique_ptr<IAppModel> IAppModel::LoadFromPmx(const Sein::Direct3D12::Device& device, const std::string& file_path)
  {
    std::unique_ptr<AppModel> app_model = nullptr;

    auto model = IModel::CreateFromPmx(file_path);
    if (!model)
    {
      return app_model;
    }

    app_model = std::make_unique<AppModel>();
    app_model->SetModel(model.release());

    // �A�v���P�[�V�����p�̒��_�f�[�^�𐶐�����
    std::vector<Vertex> vertices(app_model->GetControlPointCount());
    std::vector<uint32_t> indices(app_model->GetControlPointIndexCount());

    // ���_����
    const auto points = app_model->GetControlPoints();
    const auto normals = app_model->GetNormals();
    const auto tex_coords = app_model->GetTextureCoords();
    for (auto& vertex : vertices | boost::adaptors::indexed())
    {
      const auto index = vertex.index();

      vertex.value().position = points[index];
      vertex.value().normal = normals[index];
      vertex.value().texcoord = tex_coords[index];
    }

    // �C���f�b�N�X����
    const auto point_indices = app_model->GetControlPointIndices();
    for (auto& index : indices | boost::adaptors::indexed())
    {
      index.value() = point_indices[index.index()];
    }

    auto vertex_size = sizeof(App::IAppModel::Vertex);
    auto vertex_buffer = std::make_unique<Sein::Direct3D12::VertexBuffer>();
    auto index_buffer = std::make_unique<Sein::Direct3D12::IndexBuffer>();
    vertex_buffer->Create(&(device.GetDevice()), vertex_size * vertices.size(), vertex_size, &(vertices[0]));
    index_buffer->Create(&(device.GetDevice()), sizeof(uint32_t) * indices.size(), &(indices[0]), DXGI_FORMAT_R32_UINT);

    for (decltype(auto) vertex : vertices)
    {
      app_model->AddVertex(vertex);
    }

    app_model->AddIndex(indices);

    app_model->SetVertexBuffer(vertex_buffer.release());
    app_model->SetIndexBuffer(index_buffer.release());

    return app_model;
  }
};
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
          return nullptr;
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
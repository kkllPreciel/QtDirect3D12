/**
 *	@file		Loader.cpp
 *	@brief		Pmx読み込みに関する処理を行うプログラムソース
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
		// アラインメントを1バイトに設定
#pragma pack(push, 1)
		/**
		 *	@brief	Pmxヘッダデータ構造体
		 */
		struct Header
		{
			char	signature[4];	///< シグネチャ
			float	version;		///< バージョン
			char	globalsCount;	///< ファイル全体での設定情報の個数(エンコード方式, 追加UV数等)
			char	globals[8];		///< ファイル全体での設定情報
		};
		// アラインメントをデフォルトの設定に戻す
#pragma pack(pop)

		// アラインメントを1バイトに設定
#pragma pack(push, 1)
		/**
		 *	@brief	Pmx頂点データ構造体
		 */
		struct Vertex
		{
			DirectX::XMFLOAT3	position;	///< 座標
			DirectX::XMFLOAT3	normal;		///< 法線
			DirectX::XMFLOAT2	uv;			///< UV座標
		};
		// アラインメントをデフォルトの設定に戻す
#pragma pack(pop)

		/**
		 *	@brief	コンストラクタ
		 */
		Loader::Loader() : size(0), buffer(nullptr), header(nullptr), vertexCount(0),
			vertices(nullptr), indexCount(0), indices(nullptr), polygonCount(0)
		{

		}

		/**
		 *	@brief	デストラクタ
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
		 *	@brief	読み込みを行う
		 *	@param	filePath:読み込みを行うPmxファイルのパス
		 */
		void Loader::Load(std::string filePath)
		{
			// ファイル内容の読み込み
			{
				namespace fs = std::experimental::filesystem;

				// ファイルが存在しない
				if (false == fs::exists(filePath))
				{
					throw "Pmxファイルが存在しません。";
				}

				std::ifstream ifs(filePath, std::ios::in | std::ios::binary);

				ifs.seekg(0, std::fstream::end);
				unsigned int end = ifs.tellg();

				// 先頭に戻るために一度clear()をかける。
				// これをしないと次のseekg()でコケるときがある。
				ifs.clear();

				ifs.seekg(0, std::fstream::beg);
				size = end - ifs.tellg();

				// バッファの確保
				buffer = new char[size + 1];

				ifs.read(buffer, size);
				ifs.close();
			}

			// モデルデータ読み込み
			{
				char* buffer = this->buffer;

				// ヘッダ読み込み
				{
					// ファイルサイズがヘッダサイズ未満
					if (size < sizeof(Header))
					{
						throw "違法なデータのPmxファイルです。";
					}

					header = new Header;
					std::memcpy(header, buffer, sizeof(Header));
					buffer = buffer + sizeof(Header);

					// 違法データチェック
					// シグネチャが「PMX 」でない
					// バージョンが「2.0」か「2.1」でない
				}

				// モデル情報読み込み
				{
					// モデル名
					unsigned int nameSize = 0;
					std::memcpy(&nameSize, buffer, sizeof(nameSize));
					std::string name(buffer + sizeof(nameSize), buffer + sizeof(nameSize) + nameSize);
					buffer = buffer + sizeof(nameSize) + nameSize;

					// 英語のモデル名
					unsigned int englishNameSize = 0;
					std::memcpy(&englishNameSize, buffer, sizeof(englishNameSize));
					std::string englishName(buffer + sizeof(englishNameSize), buffer + sizeof(englishNameSize) + englishNameSize);
					buffer = buffer + sizeof(englishNameSize) + englishNameSize;

					// コメント
					unsigned int commentSize = 0;
					std::memcpy(&commentSize, buffer, sizeof(commentSize));
					std::string comment(buffer + sizeof(commentSize), buffer + sizeof(commentSize) + commentSize);
					buffer = buffer + sizeof(commentSize) + commentSize;

					// 英語のコメント
					unsigned int englishCommentSize = 0;
					std::memcpy(&englishCommentSize, buffer, sizeof(englishCommentSize));
					std::string englishComment(buffer + sizeof(englishCommentSize), buffer + sizeof(englishCommentSize) + englishCommentSize);
					buffer = buffer + sizeof(englishCommentSize) + englishCommentSize;
				}

				// 頂点データ読み込み
				{
					vertexCount = 0;
					std::memcpy(&vertexCount, buffer, sizeof(vertexCount));
					buffer = buffer + sizeof(vertexCount);

					unsigned int addUvCount = header->globals[1];
					unsigned int boneIndexSize = header->globals[5];
					vertices = new Vertex[vertexCount];
					for (int i = 0; i < vertexCount; ++i)
					{
						// 座標
						std::memcpy(&(vertices[i].position), buffer, sizeof(vertices[i].position));
						buffer = buffer + sizeof(vertices[i].position);

						// 法線
						std::memcpy(&(vertices[i].normal), buffer, sizeof(vertices[i].normal));
						buffer = buffer + sizeof(vertices[i].normal);

						// UV
						std::memcpy(&(vertices[i].uv), buffer, sizeof(vertices[i].uv));
						buffer = buffer + sizeof(vertices[i].uv);

						// 追加UV
						if (0 < addUvCount)
						{
							buffer = buffer + (sizeof(DirectX::XMFLOAT4) * addUvCount);
						}

						// ウェイト変更方式
						{
							unsigned char weightType = 0;
							std::memcpy(&weightType, buffer, sizeof(weightType));
							buffer = buffer + sizeof(weightType);

							// ウェイト種別毎のデータ読み込み
							// 今回は使用しないのでスキップする
							// TODO:State or Strategyパターン作成
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
								throw "違法なウェイト変更方式データです。";
								break;
							}
						}

						// エッジ倍率
						// 使用しないのでスキップ
						buffer = buffer + sizeof(float);
					}
				}

				// 頂点インデックスデータ読み込み
				{
					indexCount = 0;
					std::memcpy(&indexCount, buffer, sizeof(indexCount));
					buffer = buffer + sizeof(indexCount);

					unsigned int vertexIndexSize = header->globals[2];
					indices = new unsigned int[indexCount];

					std::memcpy(indices, buffer, vertexIndexSize * indexCount);
					buffer = buffer + (vertexIndexSize * indexCount);
				}

				// テクスチャ読み込み
				{
					unsigned int textureCount = 0;
					std::memcpy(&textureCount, buffer, sizeof(textureCount));
					buffer = buffer + sizeof(textureCount);

					for (unsigned int i = 0; i < textureCount; ++i)
					{
						// テクスチャパスサイズ
						unsigned int texturePathSize = 0;
						std::memcpy(&texturePathSize, buffer, sizeof(texturePathSize));
						buffer = buffer + sizeof(texturePathSize);

						// テクスチャ
						buffer = buffer + texturePathSize;
					}
				}

				// マテリアル読み込み
				{
					unsigned int materialCount = 0;
					std::memcpy(&materialCount, buffer, sizeof(materialCount));
					buffer = buffer + sizeof(materialCount);

					for (int i = 0; i < materialCount; ++i)
					{
						// マテリアル名
						unsigned int nameSize = 0;
						std::memcpy(&nameSize, buffer, sizeof(nameSize));
						std::string name(buffer + sizeof(nameSize), buffer + sizeof(nameSize) + nameSize);
						buffer = buffer + sizeof(nameSize) + nameSize;

						// マテリアル名(英語)
						unsigned int englishNameSize = 0;
						std::memcpy(&englishNameSize, buffer, sizeof(englishNameSize));
						std::string englishName(buffer + sizeof(englishNameSize), buffer + sizeof(englishNameSize) + englishNameSize);
						buffer = buffer + sizeof(englishNameSize) + englishNameSize;

						// 拡散反射光(Diffuse)
						buffer = buffer + sizeof(DirectX::XMFLOAT4);

						// 鏡面反射光(Specular)
						buffer = buffer + sizeof(DirectX::XMFLOAT3);

						// 鏡面反射の強度
						buffer = buffer + sizeof(float);

						// 環境光(Ambient)
						buffer = buffer + sizeof(DirectX::XMFLOAT3);

						// 描画フラグ
						buffer = buffer + sizeof(unsigned char);

						// エッジ色
						buffer = buffer + sizeof(DirectX::XMFLOAT4);

						// エッジサイズ
						buffer = buffer + sizeof(float);

						// 通常テクスチャ
						buffer = buffer + header->globals[3];

						// スフィアテクスチャ
						buffer = buffer + header->globals[3];

						// スフィアモード
						buffer = buffer + sizeof(unsigned char);

						// 共有Toonフラグ
						bool is_share = false;
						std::memcpy(&is_share, buffer, sizeof(is_share));
						buffer = buffer + sizeof(unsigned char);

						buffer = buffer + (is_share ? sizeof(unsigned char) : header->globals[3]);

						// メモ
						unsigned int memoSize = 0;
						std::memcpy(&memoSize, buffer, sizeof(memoSize));
						std::string memoName(buffer + sizeof(memoSize), buffer + sizeof(memoSize) + memoSize);
						buffer = buffer + sizeof(memoSize) + memoSize;

						// マテリアルに対応する頂点インデックス数
						unsigned int vetexCount = 0;
						std::memcpy(&vetexCount, buffer, sizeof(vetexCount));
						buffer = buffer + sizeof(vetexCount);
					}
				}

				// ポリゴン数
				polygonCount = indexCount / 3;
			}
		}

		/**
		 *	@brief	頂点数を取得する
		 *	@return	頂点数
		 */
		unsigned int Loader::GetVertexCount() const
		{
			return vertexCount;
		}

		/**
		 *	@brief	頂点データを取得する
		 *	@return	頂点データへのポインタ
		 */
		const void* const Loader::GetVertices() const
		{
			return vertices;
		}

		/**
		 *	@brief	頂点のサイズを取得する
		 *	@return	頂点のサイズ
		 */
		unsigned int Loader::GetVertexSize() const
		{
			return sizeof(Vertex);
		}

		/**
		 *	@brief	頂点インデックス数を取得する
		 *	@return	頂点インデックス数
		 */
		unsigned int Loader::GetIndexCount() const
		{
			return indexCount;
		}

		/**
		 *	@brief	頂点インデックスデータを取得する
		 *	@return	頂点インデックスデータへのポインタ
		 */
		const void* const Loader::GetIndices() const
		{
			return indices;
		}

		/**
		 *	@brief	頂点インデックスのサイズを取得する
		 *	@return	頂点インデックスのサイズ
		 */
		const unsigned int Loader::GetIndexSize() const
		{
			return header->globals[2];
		}
	};
};
#include "particle/model/model.hpp"
#include <cstddef>
#include "core/texture/texture.hpp"
#include "glad/glad.h"
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/types.h"
#include "assimp/vector3.h"
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"
#include "core/shader/shader.hpp"
#include "core/vertex/vertex.hpp"
#include "core/world/world.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "particle/color_id/color_id.hpp"

Particle::Model::Model() {}

void Particle::Model::StoreMeshes(
    std::vector<std::shared_ptr<Core::Object>>& storage) {
  for (std::shared_ptr<Core::Object>& obj : objects) {
    storage.emplace_back(obj);
  }
}

void Particle::Model::LoadModel(const std::string& path) {
  Assimp::Importer importer;

  // TODO: Flip UV disabled because of jingliu model
  const aiScene* scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace);
  if (!scene || !scene->mRootNode ||
      scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
    throw std::runtime_error(importer.GetErrorString());
  }
  directory = path.substr(0, path.find_last_of("/"));

  processScene(scene->mRootNode, scene);
}

void Particle::Model::processScene(aiNode* node, const aiScene* scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    processMesh(scene->mMeshes[node->mMeshes[i]], scene, ColorID::Generate());
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    processScene(node->mChildren[i], scene);
  }
}

void Particle::Model::processMesh(aiMesh* mesh, const aiScene* scene,
                                  glm::vec3 objectColor) {
  std::shared_ptr<Core::Object> object = std::make_shared<Core::Object>();
  // FIXME: Remove
  /*object->transform->position.z = -4.0f;*/
  object->name = mesh->mName.C_Str();
  objects.emplace_back(object);

  Core::Shader shader{"./shaders/diffuse/diffuse_v2.vert",
                      "./shaders/sketch/sketch.frag"};
  /*Core::Shader shader{"./shaders/diffuse/diffuse_v2.vert",*/
  /*                    "./shaders/video-demo/diffuse.frag"};*/
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> objectMesh =
      object->AddComponent<Core::Mesh>(material);
  object->mesh = objectMesh;

  for (size_t i = 0; i < mesh->mNumVertices; i++) {
    aiVector3D vert = mesh->mVertices[i];
    aiVector3D norm = mesh->mNormals[i];

    Core::VertexData vertex;
    vertex.color = objectColor;
    // TODO: Flipped the z and y coordinate because of jingliu model
    vertex.SetPosition(glm::vec3(vert.x, vert.y, vert.z));
    vertex.SetNormal(glm::vec3(norm.x, norm.y, norm.z));
    if (mesh->mTextureCoords[0]) {
      vertex.SetTextureCoordinate(glm::vec2(mesh->mTextureCoords[0][i].x,
                                            mesh->mTextureCoords[0][i].y));
    } else {
      vertex.SetTextureCoordinate(glm::vec2(0.0f));
    }
    vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y,
                               mesh->mTangents[i].z);

    objectMesh->AddVertex(vertex);
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++) {
      objectMesh->AddIndex(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    /*loadTextures(material, aiTextureType_DIFFUSE, objectMesh,*/
    /*"material.diffuse");*/
    /*loadTextures(material, aiTextureType_SPECULAR, objectMesh,*/
    /*"material.specular");*/
    /*loadTextures(material, aiTextureType_HEIGHT, objectMesh, */
    /*"material.normal");*/
  }

  objectMesh->SetupMesh();
  Core::World::AddObject(object);
}

void Particle::Model::loadTextures(aiMaterial* material, aiTextureType type,
                                   std::shared_ptr<Core::Mesh>& objectMesh,
                                   const std::string& uniform) {
  int colorSpace = GL_RGB;
  int colorCode = GL_RGB;
  for (size_t i = 0; i < material->GetTextureCount(type); i++) {
    aiString str;
    material->GetTexture(type, i, &str);
    if (type == aiTextureType_DIFFUSE) {
      colorSpace = GL_SRGB;
    }
    std::string name = std::string(str.C_Str());
    if (name.contains('\\')) {
      name = name.substr(name.find_last_of("\\") + 1, std::string::npos);
    }

    const std::string path =
        std::format("{}/{}", directory.c_str(), name.c_str());
    Core::Create2DTextureFromImageDetail detail;
    detail.path = path;
    detail.colorCode = colorCode;
    detail.colorSpace = colorSpace;
    Core::Texture texture =
        Core::Texture::Create2DTextureFromImage(path, detail);
    objectMesh->material->SetInt(uniform, Core::Texture::GetTextureID(path));
    objectMesh->material->AddTexture(texture);
  }
}

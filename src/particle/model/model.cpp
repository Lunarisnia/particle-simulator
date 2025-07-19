#include "particle/model/model.hpp"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/vector3.h"
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"
#include "core/shader/shader.hpp"
#include "core/vertex/vertex.hpp"
#include "core/world/world.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

void Particle::Model::LoadModel(const std::string& path) {
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
  if (!scene || !scene->mRootNode ||
      scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
    throw std::runtime_error(importer.GetErrorString());
  }

  processScene(scene->mRootNode, scene);
}

void Particle::Model::processScene(aiNode* node, const aiScene* scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    processMesh(scene->mMeshes[node->mMeshes[i]]);
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    processScene(node->mChildren[i], scene);
  }
}

void Particle::Model::processMesh(aiMesh* mesh) {
  std::shared_ptr<Core::Object> object = std::make_shared<Core::Object>();
  object->name = mesh->mName.C_Str();

  Core::Shader shader{"./shaders/diffuse/diffuse.vert",
                      "./shaders/diffuse/solid.frag"};
  std::shared_ptr<Core::Material> material =
      std::make_shared<Core::Material>(shader);
  std::shared_ptr<Core::Mesh> objectMesh =
      object->AddComponent<Core::Mesh>(material);
  object->mesh = objectMesh;

  for (size_t i = 0; i < mesh->mNumVertices; i++) {
    aiVector3D vert = mesh->mVertices[i];
    aiVector3D norm = mesh->mNormals[i];

    Core::VertexData vertex;
    vertex.SetPosition(glm::vec3(vert.x, vert.y, vert.z));
    vertex.SetNormal(glm::vec3(norm.x, norm.y, norm.z));
    if (mesh->mTextureCoords[0]) {
      vertex.SetTextureCoordinate(glm::vec2(mesh->mTextureCoords[0][i].x,
                                            mesh->mTextureCoords[0][i].y));
    } else {
      vertex.SetTextureCoordinate(glm::vec2(0.0f));
    }

    objectMesh->AddVertex(vertex);
    // TODO: load textures
  }

  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++) {
      objectMesh->AddIndex(face.mIndices[j]);
    }
  }

  objectMesh->SetupMesh();
  Core::World::AddObject(object);
}

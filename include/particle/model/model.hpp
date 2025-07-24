#pragma once
#include <memory>
#include <string>
#include <vector>
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"
namespace Particle {
class Model {
 private:
  std::string directory;

 public:
  std::vector<std::shared_ptr<Core::Object>> objects;
  void LoadModel(const std::string &path);
  void StoreMeshes(std::vector<std::shared_ptr<Core::Object>> &storage);

 private:
  void processScene(aiNode *node, const aiScene *scene);
  void processMesh(aiMesh *mesh, const aiScene *scene);
  void loadTextures(aiMaterial *material, aiTextureType type,
                    std::shared_ptr<Core::Mesh> &objectMesh,
                    const std::string &uniform);

 public:
  Model();
};
};  // namespace Particle

#pragma once
#include <memory>
#include <string>
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "core/components/mesh.hpp"
namespace Particle {
class Model {
 private:
  static std::string directory;

 public:
  // TODO: load model, create a new object for each mesh, assign all the
  // required information to those
  static void LoadModel(const std::string &path);

 private:
  static void processScene(aiNode *node, const aiScene *scene);
  static void processMesh(aiMesh *mesh, const aiScene *scene);
  static void loadTextures(aiMaterial *material, aiTextureType type,
                           std::shared_ptr<Core::Mesh> &objectMesh);
};
};  // namespace Particle

#pragma once
#include <string>
#include "assimp/mesh.h"
#include "assimp/scene.h"
namespace Particle {
class Model {
 public:
  // TODO: load model, create a new object for each mesh, assign all the
  // required information to those
  static void LoadModel(const std::string &path);

 private:
  static void processScene(aiNode *node, const aiScene *scene);
  static void processMesh(aiMesh *mesh);
};
};  // namespace Particle

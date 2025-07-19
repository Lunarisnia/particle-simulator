#pragma once
#include <string>
namespace Particle {
class Model {
 public:
  // TODO: load model, create a new object for each mesh, assign all the
  // required information to those
  static void LoadModel(const std::string &path);
};
};  // namespace Particle

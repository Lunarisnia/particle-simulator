#pragma once
namespace Core {
class VertexData {
  float x;
  float y;
  float z;

 public:
  inline VertexData(float x, float y, float z) : x(x), y(y), z(z) {};
};
};  // namespace Core

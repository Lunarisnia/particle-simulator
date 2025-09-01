#include "core/procedural/procedural.hpp"
#include <cstddef>
#include "core/math/math.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

const float PI = 3.14159265358979323846f;

void Core::Procedural::GenerateComplexPlane(std::shared_ptr<Mesh> &mesh,
                                            int div) {
  float scale = 10.0f;
  glm::vec3 v0 = glm::vec3(-0.5f, -0.5f, 0.0f) * scale;
  glm::vec3 v1 = glm::vec3(-0.5f, 0.5f, 0.0f) * scale;
  glm::vec3 v2 = glm::vec3(0.5f, 0.5f, 0.0f) * scale;
  glm::vec3 v4 = glm::vec3(0.5f, -0.5f, 0.0f) * scale;

  glm::vec3 v10 = (v0 - v1) / float(div);
  glm::vec3 v24 = (v4 - v2) / float(div);

  for (int i = 0; i <= div; i++) {
    glm::vec3 start = v1 + v10 * float(i);
    glm::vec3 end = v2 + v24 * float(i);

    glm::vec3 step = (end - start) / float(div);
    for (int j = 0; j <= div; j++) {
      glm::vec3 pos = start + step * float(j);

      glm::vec2 uv(float(j) / div, 1.0f - float(i) / div);

      mesh->AddVertex(Core::VertexData{
          pos,
          uv,
          glm::vec3(0.0f, 0.0f, 1.0f),
      });
    }
  }

  for (int i = 0; i < div; i++) {
    for (int j = 0; j < div; j++) {
      int row1 = i * (div + 1);
      int row2 = (i + 1) * (div + 1);

      int topLeft = row1 + j;
      int topRight = row1 + j + 1;
      int bottomLeft = row2 + j;
      int bottomRight = row2 + j + 1;

      mesh->AddIndex(topLeft);
      mesh->AddIndex(bottomLeft);
      mesh->AddIndex(topRight);
      glm::vec3 tangent1 = Core::Math::CalculateTangent(
          *mesh->GetVertex(topLeft), *mesh->GetVertex(bottomLeft),
          *mesh->GetVertex(topRight));
      mesh->GetVertex(topLeft)->tangent = tangent1;
      mesh->GetVertex(bottomLeft)->tangent = tangent1;
      mesh->GetVertex(topRight)->tangent = tangent1;

      mesh->AddIndex(topRight);
      mesh->AddIndex(bottomLeft);
      mesh->AddIndex(bottomRight);
      glm::vec3 tangent2 = Core::Math::CalculateTangent(
          *mesh->GetVertex(topRight), *mesh->GetVertex(bottomLeft),
          *mesh->GetVertex(bottomRight));
      mesh->GetVertex(bottomRight)->tangent = tangent2;
    }
  }

  mesh->SetupMesh();
}

void Core::Procedural::GeneratePlane(std::shared_ptr<Mesh> &mesh) {
  mesh->AddVertex(Core::VertexData{
      glm::vec3(-0.5f, 0.5f, 0.0f),
      glm::vec2(0.0f, 1.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
  });
  mesh->AddVertex(Core::VertexData{
      glm::vec3(-0.5f, -0.5f, 0.0f),
      glm::vec2(0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
  });
  mesh->AddVertex(Core::VertexData{
      glm::vec3(0.5f, -0.5f, 0.0f),
      glm::vec2(1.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
  });
  mesh->AddVertex(Core::VertexData{
      glm::vec3(0.5f, 0.5f, 0.0f),
      glm::vec2(1.0f, 1.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
  });
  mesh->AddIndex(0);
  mesh->AddIndex(1);
  mesh->AddIndex(2);
  mesh->AddIndex(2);
  mesh->AddIndex(3);
  mesh->AddIndex(0);

  glm::vec3 tangent1 = Core::Math::CalculateTangent(
      *mesh->GetVertex(0), *mesh->GetVertex(1), *mesh->GetVertex(2));
  glm::vec3 tangent2 = Core::Math::CalculateTangent(
      *mesh->GetVertex(0), *mesh->GetVertex(2), *mesh->GetVertex(3));

  mesh->GetVertex(0)->tangent = tangent1;
  mesh->GetVertex(1)->tangent = tangent1;
  mesh->GetVertex(2)->tangent = tangent1;
  mesh->GetVertex(3)->tangent = tangent2;

  mesh->SetupMesh();
}

void Core::Procedural::GenerateCube(std::shared_ptr<Mesh> &mesh) {
  Core::VertexData vData;
  // Front face (z = 0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, 0.5f, 0.5f));  // Top right
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    vData.SetNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(0);

    vData.SetPosition(glm::vec3(0.5f, -0.5f, 0.5f));  // Bottom right
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(1);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, 0.5f));  // Bottom left
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(2);

    mesh->AddIndex(0);
    mesh->AddIndex(2);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, 0.5f));  // Top left
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(3);
  }

  // Right face (x = 0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, -0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    vData.SetNormal(glm::vec3(1.0f, 0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(4);

    vData.SetPosition(glm::vec3(0.5f, 0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(5);

    vData.SetPosition(glm::vec3(0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(6);

    vData.SetPosition(glm::vec3(0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(7);

    mesh->AddIndex(6);
    mesh->AddIndex(5);
  }

  // Left face (x = -0.5)
  {
    vData.SetPosition(glm::vec3(-0.5f, -0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    vData.SetNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(8);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(9);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(10);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(11);

    mesh->AddIndex(10);
    mesh->AddIndex(9);
  }

  // Back face (z = -0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    vData.SetNormal(glm::vec3(0.0f, 0.0f, -1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(12);

    vData.SetPosition(glm::vec3(0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(13);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(14);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(15);

    mesh->AddIndex(14);
    mesh->AddIndex(13);
  }

  // Up face (y = 0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, 0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    vData.SetNormal(glm::vec3(0.0f, 1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(16);

    vData.SetPosition(glm::vec3(0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(17);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(18);

    vData.SetPosition(glm::vec3(-0.5f, 0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(19);

    mesh->AddIndex(18);
    mesh->AddIndex(17);
  }

  // Down face (y = -0.5)
  {
    vData.SetPosition(glm::vec3(0.5f, -0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 0.0f));
    vData.SetNormal(glm::vec3(0.0f, -1.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(20);

    vData.SetPosition(glm::vec3(0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(1.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(21);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, 0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 0.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(22);

    vData.SetPosition(glm::vec3(-0.5f, -0.5f, -0.5f));
    vData.SetTextureCoordinate(glm::vec2(0.0f, 1.0f));
    mesh->AddVertex(vData);
    mesh->AddIndex(23);

    mesh->AddIndex(22);
    mesh->AddIndex(21);
  }

  for (size_t i = 0; i < mesh->GetVertexLength(); i += 4) {
    size_t first = i;
    size_t second = i + 1;
    size_t third = i + 2;
    size_t fourth = i + 3;

    glm::vec3 tangent1 = Core::Math::CalculateTangent(*mesh->GetVertex(first),
                                                      *mesh->GetVertex(second),
                                                      *mesh->GetVertex(third));
    glm::vec3 tangent2 = Core::Math::CalculateTangent(*mesh->GetVertex(fourth),
                                                      *mesh->GetVertex(second),
                                                      *mesh->GetVertex(third));
    mesh->GetVertex(first)->tangent = tangent1;
    mesh->GetVertex(second)->tangent = tangent1;
    mesh->GetVertex(third)->tangent = tangent1;
    mesh->GetVertex(fourth)->tangent = tangent2;
  }

  mesh->SetupMesh();
}

void Core::Procedural::GenerateUVSphere(std::shared_ptr<Mesh> &mesh,
                                        int sectorCount, int stackCount,
                                        float radius) {
  for (int i = 0; i <= stackCount; ++i) {
    float stackAngle = PI / 2 - i * (PI / stackCount);  // from +pi/2 to -pi/2
    float xy = radius * cosf(stackAngle);               // r * cos(phi)
    float y = radius * sinf(stackAngle);                // r * sin(phi)

    for (int j = 0; j <= sectorCount; ++j) {
      Core::VertexData vertex;
      float sectorAngle = j * 2 * PI / sectorCount;  // from 0 to 2pi

      float x = xy * cosf(sectorAngle);  // x = r * cos(phi) * cos(theta)
      float z = xy * sinf(sectorAngle);  // y = r * cos(phi) * sin(theta)

      float u = static_cast<float>(j) / sectorCount;
      float v = static_cast<float>(i) / stackCount;

      glm::vec3 position(x, y, z);
      glm::vec3 normal = glm::normalize(position);
      glm::vec2 texCoord(u, v);

      vertex.position = position;
      vertex.normal = normal;
      vertex.textureCoord = texCoord;
      mesh->AddVertex(vertex);
    }
  }

  for (int i = 0; i < stackCount; ++i) {
    for (int j = 0; j < sectorCount; ++j) {
      int first = i * (sectorCount + 1) + j;
      int second = first + sectorCount + 1;

      mesh->AddIndex(first);
      mesh->AddIndex(second);
      mesh->AddIndex(first + 1);
      glm::vec3 tangent1 = Core::Math::CalculateTangent(
          *mesh->GetVertex(first), *mesh->GetVertex(second),
          *mesh->GetVertex(first + 1));
      mesh->GetVertex(first)->tangent = tangent1;
      mesh->GetVertex(second)->tangent = tangent1;
      mesh->GetVertex(first + 1)->tangent = tangent1;

      mesh->AddIndex(first + 1);
      mesh->AddIndex(second);
      mesh->AddIndex(second + 1);
      glm::vec3 tangent2 = Core::Math::CalculateTangent(
          *mesh->GetVertex(first + 1), *mesh->GetVertex(second),
          *mesh->GetVertex(second + 1));
      mesh->GetVertex(second + 1)->tangent = tangent2;
    }
  }

  mesh->SetupMesh();
}

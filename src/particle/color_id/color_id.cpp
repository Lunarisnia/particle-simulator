#include "particle/color_id/color_id.hpp"
#include <cstddef>
#include "glm/ext/vector_float3.hpp"

size_t Particle::ColorID::nextColorIndex = 0;
std::vector<glm::vec3> Particle::ColorID::colors = {
    glm::vec3(1.0, 0.0, 0.0),   // red
    glm::vec3(0.0, 1.0, 0.0),   // green
    glm::vec3(0.0, 0.0, 1.0),   // blue
    glm::vec3(1.0, 1.0, 0.0),   // yellow
    glm::vec3(1.0, 0.0, 1.0),   // magenta
    glm::vec3(0.0, 1.0, 1.0),   // cyan
    glm::vec3(1.0, 0.5, 0.0),   // orange
    glm::vec3(0.5, 0.0, 1.0),   // purple
    glm::vec3(0.0, 0.5, 1.0),   // azure
    glm::vec3(0.5, 1.0, 0.0),   // lime
    glm::vec3(1.0, 0.0, 0.5),   // rose
    glm::vec3(0.0, 1.0, 0.5),   // seafoam
    glm::vec3(0.75, 1.0, 0.0),  // chartreuse
    glm::vec3(0.4, 0.8, 1.0),   // sky blue
    glm::vec3(1.0, 0.6, 0.6),   // salmon
    glm::vec3(0.5, 0.5, 0.0),   // olive
    glm::vec3(0.0, 0.5, 0.5),   // teal
    glm::vec3(0.5, 0.0, 0.5),   // violet
    glm::vec3(0.9, 0.3, 0.2),   // vermilion
    glm::vec3(0.2, 0.6, 0.2),   // grass green
    glm::vec3(0.3, 0.3, 0.9),   // ultramarine
    glm::vec3(1.0, 0.8, 0.0),   // gold
    glm::vec3(0.6, 0.4, 0.8),   // lavender
    glm::vec3(0.9, 0.7, 0.5),   // peach
    glm::vec3(0.1, 0.9, 0.7),   // mint
    glm::vec3(0.8, 0.1, 0.7),   // fuchsia
    glm::vec3(0.3, 0.7, 0.9),   // iceberg
    glm::vec3(0.7, 0.9, 0.3),   // pistachio
    glm::vec3(0.8, 0.2, 0.1),   // brick red
    glm::vec3(0.2, 0.1, 0.8)    // deep indigo
};

glm::vec3 Particle::ColorID::Generate() {
  glm::vec3 color = colors[nextColorIndex % colors.size()];
  nextColorIndex++;
  return color;
}

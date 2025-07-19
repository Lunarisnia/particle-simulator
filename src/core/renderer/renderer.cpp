#include "core/renderer/renderer.hpp"
#include <memory>
#include <stdexcept>
#include <vector>
#include "core/components/mesh.hpp"
#include "core/object/object.hpp"
#include "core/static_camera/static_camera.hpp"
#include "core/window/window.hpp"

std::vector<std::shared_ptr<Core::Mesh>> Core::Renderer::renderQueue;

void Core::Renderer::Init() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("failed to initiate GLAD");
  }

#if defined(WIN32)
  glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
#else
  glViewport(0, 0, Window::GetWidth() * 2, Window::GetHeight() * 2);
#endif

  /*SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);*/
  glEnable(GL_DEPTH_TEST);
}

void Core::Renderer::Render() {
  for (std::shared_ptr<Mesh> &mesh : renderQueue) {
    if (!mesh->isActive) {
      continue;
    }
    Object *owner = mesh->GetOwner();
    if (!owner) {
      continue;
    }

    mesh->material->Use();
    mesh->material->SetMat4("model", owner->transform->GetTransformMatrix());
    mesh->material->SetMat4("view", StaticCamera::GetViewMatrix());
    mesh->material->SetMat4("projection", StaticCamera::GetProjectionMatrix());
    mesh->BindVertexArray();

    glDrawElements(GL_TRIANGLES, mesh->GetIndiceLength(), GL_UNSIGNED_INT, 0);
  }
}

void Core::Renderer::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void Core::Renderer::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Core::Renderer::AddToRenderQueue(std::shared_ptr<Mesh> mesh) {
  renderQueue.emplace_back(mesh);
}

void Core::Renderer::Cleanup() {}

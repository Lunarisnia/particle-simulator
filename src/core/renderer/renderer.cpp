#include "core/renderer/renderer.hpp"
#include <stdexcept>
#include "core/shader/shader.hpp"
#include "core/window/window.hpp"

unsigned int Core::Renderer::shaderProgram = 0;
unsigned int Core::Renderer::VAO = 0;
unsigned int Core::Renderer::VBO = 0;
unsigned int Core::Renderer::EBO = 0;
Core::Shader *Core::Renderer::program;

void Core::Renderer::Init() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("failed to initiate GLAD");
  }

#if defined(WIN32)
  glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
#else
  glViewport(0, 0, Window::GetWidth() * 2, Window::GetHeight() * 2);
#endif

  glEnable(GL_DEPTH_TEST);
  triangle();
}

void Core::Renderer::Render() { renderTriangle(); }

void Core::Renderer::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void Core::Renderer::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Core::Renderer::Cleanup() {}

void Core::Renderer::triangle() {
  program = new Shader("./shaders/diffuse/diffuse.vert",
                       "./shaders/diffuse/diffuse.frag");

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  float vertices[] = {
      0.5f,  -0.5f, 0.0f,  //
      -0.5f, -0.5f, 0.0f,  //
      0.5f,  0.5f,  0.0f,  //
      -0.5f, 0.5f,  0.0f,  //
  };
  int indice[] = {
      0, 1, 2,  //
      3, 1, 2,
  };

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
  glEnableVertexAttribArray(0);
}

void Core::Renderer::renderTriangle() {
  program->Use();
  glBindVertexArray(
      VAO);  // seeing as we only have a single VAO there's no need to bind it
             // every time, but we'll do so to keep things a bit more organized
  // glDrawArrays(GL_TRIANGLES, 0, 6);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

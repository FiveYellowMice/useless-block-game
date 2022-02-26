#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ApplicationException.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "main.hpp"
#include "build_config.h"

Vertex vertices[] = {
  { -1.f, -1.f,  1.f,  0.f,  0.f,  1.f, 0.5f, 0.5f },
  { -1.f,  1.f,  1.f,  0.f,  0.f,  1.f, 0.5f, 0.0f },
  {  1.f, -1.f,  1.f,  0.f,  0.f,  1.f, 1.0f, 0.5f },
  {  1.f,  1.f,  1.f,  0.f,  0.f,  1.f, 1.0f, 0.0f },

  { -1.f, -1.f, -1.f,  0.f,  0.f, -1.f, 0.5f, 0.5f },
  { -1.f,  1.f, -1.f,  0.f,  0.f, -1.f, 0.5f, 0.0f },
  {  1.f, -1.f, -1.f,  0.f,  0.f, -1.f, 1.0f, 0.5f },
  {  1.f,  1.f, -1.f,  0.f,  0.f, -1.f, 1.0f, 0.0f },

  {  1.f, -1.f, -1.f,  1.f,  0.f,  0.f, 0.5f, 0.5f },
  {  1.f,  1.f, -1.f,  1.f,  0.f,  0.f, 0.5f, 0.0f },
  {  1.f, -1.f,  1.f,  1.f,  0.f,  0.f, 1.0f, 0.5f },
  {  1.f,  1.f,  1.f,  1.f,  0.f,  0.f, 1.0f, 0.0f },

  { -1.f, -1.f, -1.f, -1.f,  0.f,  0.f, 0.5f, 0.5f },
  { -1.f,  1.f, -1.f, -1.f,  0.f,  0.f, 0.5f, 0.0f },
  { -1.f, -1.f,  1.f, -1.f,  0.f,  0.f, 1.0f, 0.5f },
  { -1.f,  1.f,  1.f, -1.f,  0.f,  0.f, 1.0f, 0.0f },

  { -1.f,  1.f,  1.f,  0.f,  1.f,  0.f, 0.0f, 0.0f },
  { -1.f,  1.f, -1.f,  0.f,  1.f,  0.f, 0.0f, 0.5f },
  {  1.f,  1.f,  1.f,  0.f,  1.f,  0.f, 0.5f, 0.0f },
  {  1.f,  1.f, -1.f,  0.f,  1.f,  0.f, 0.5f, 0.5f },

  { -1.f, -1.f,  1.f,  0.f, -1.f,  0.f, 0.0f, 0.0f },
  { -1.f, -1.f, -1.f,  0.f, -1.f,  0.f, 0.0f, 1.0f },
  {  1.f, -1.f,  1.f,  0.f, -1.f,  0.f, 0.5f, 0.5f },
  {  1.f, -1.f, -1.f,  0.f, -1.f,  0.f, 0.5f, 1.0f },
};

GLuint vertexIndices[] = {
   3,  1,  2,  2,  1,  0,
   4,  5,  6,  6,  5,  7,
   8,  9, 10, 10,  9, 11,
  15, 13, 14, 14, 13, 12,
  19, 17, 18, 18, 17, 16,
  20, 21, 22, 22, 21, 23,
};

int main(int argc, char* argv[]) {
  (void) argc;
  (void) argv;

  try {
    glfwSetErrorCallback([] (int error, const char* description) {
      std::cerr << "GLFW Error: " << description << std::endl;
    });

    if (!glfwInit()) throw ApplicationException("Cannot initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
    if (!window) throw ApplicationException("Cannot create window");
    glfwMakeContextCurrent(window);

    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) throw ApplicationException((const char*) glewGetErrorString(glewErr));

//     if (!glewIsSupported("GL_ARB_texture_storage")) {
//       throw ApplicationException("Necessary OpenGL extensions not supported");
//     }

    glfwSetFramebufferSizeCallback(window, [] (GLFWwindow* window, int width, int height) {
      glViewport(0, 0, width, height);
    });

    glfwSetKeyCallback(window, [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
      switch (key) {
        case GLFW_KEY_ESCAPE:
          if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
          }
          break;
        default:
          ;
      }
    });

    glfwSwapInterval(1);
    glEnable(GL_CULL_FACE);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vertexIndexBufferId;
    glGenBuffers(1, &vertexIndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndices), vertexIndices, GL_STATIC_DRAW);

    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.loadFromFile("shaders/vert.glsl");
    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.loadFromFile("shaders/frag.glsl");

    Texture grassBlockTexture(GL_TEXTURE_2D);
    grassBlockTexture.loadFromFile("textures/grass_block.png");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShader.id);
    glAttachShader(shaderProgramId, fragmentShader.id);
    glLinkProgram(shaderProgramId);
    GLint programLinkStatus;
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &programLinkStatus);
    if (!programLinkStatus) throw ApplicationException("Program linking failed");

    GLint mvpLocation = glGetUniformLocation(shaderProgramId, "MVP");
    GLint colorMapLocation = glGetUniformLocation(shaderProgramId, "colorMap");
    GLint vPosLocation = glGetAttribLocation(shaderProgramId, "vPos");
    GLint vNormLocation = glGetAttribLocation(shaderProgramId, "vNorm");
    GLint vTexCoordLocation = glGetAttribLocation(shaderProgramId, "vTexCoord");

    glEnableVertexAttribArray(vPosLocation);
    glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, x));
    glEnableVertexAttribArray(vNormLocation);
    glVertexAttribPointer(vNormLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, nx));
    glEnableVertexAttribArray(vTexCoordLocation);
    glVertexAttribPointer(vTexCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, u));

    while (!glfwWindowShouldClose(window)) {
      float ratio;
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float) height;

      glClear(GL_COLOR_BUFFER_BIT);

      glActiveTexture(GL_TEXTURE0);
      grassBlockTexture.bind();

      glm::mat4 m = glm::rotate(glm::mat4(1.f), static_cast<float>(glfwGetTime()) * 2.f * 0.1f * glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f));
      glm::mat4 v = glm::scale(glm::rotate(glm::mat4(1.f), glm::pi<float>() / 9.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(0.6f));
      glm::mat4 p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      glm::mat4 mvp = p * v * m;

      glUseProgram(shaderProgramId);
      glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
      glUniform1i(colorMapLocation, 0);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

  } catch (const ApplicationException& err) {
    std::cerr << "Error: " << err.what() << std::endl;

    glfwTerminate();
    exit(-1);
  }
}

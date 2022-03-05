#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ApplicationException.hpp"
#include "Shader.hpp"
#include "StreamingTextures.hpp"
#include "Block.hpp"
#include "BlockType.hpp"
#include "BlocksMap.hpp"
#include "BlocksMesh.hpp"
#include "Vertex.hpp"
#include "Entity.hpp"
#include "build_config.h"

float lastFrameTime;
float deltaFrameTime;
glm::vec2 lastMousePos;

Entity player("player");

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

    if (!glewIsSupported("GL_ARB_texture_storage")) {
      throw ApplicationException("Necessary OpenGL extensions not supported");
    }

    if (GLEW_KHR_debug) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false); // disable notifications
      glDebugMessageCallback([] (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        switch (severity) {
          case GL_DEBUG_SEVERITY_HIGH:
            std::cerr << "GL Error: ";
            break;
          case GL_DEBUG_SEVERITY_MEDIUM:
            std::cerr << "GL Warning: ";
            break;
          default:
            std::cerr << "GL Message: ";
        }
        std::cerr << "(" << id << ") " << message << std::endl;
      }, nullptr);
    } else {
      std::cerr << "Debug output unavailable" << std::endl;
    }

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, [] (GLFWwindow* window, double xpos, double ypos) {
      glm::vec2 currentMousePos((float) xpos, (float) ypos);
      glm::vec2 deltaMousePos = currentMousePos - lastMousePos;
      lastMousePos = currentMousePos;

      glm::vec2 playerRotation = player.rotation();
      playerRotation.y = playerRotation.y + deltaMousePos.x * 0.001f;
      playerRotation.x = glm::clamp(playerRotation.x - deltaMousePos.y * 0.001f, -glm::pi<float>() / 2 + 0.001f, glm::pi<float>() / 2 - 0.001f);
      player.rotation(playerRotation);
    });

    glfwSwapInterval(1);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Define block types, load block textures

    StreamingTextures blockTextures(16, 16, std::vector<GLenum>{GL_RGBA8}, [] (size_t i, GLuint textureId) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    });

    std::vector<std::unique_ptr<BlockType>> blockTypes;

    {
      auto grassBlockTextureTop = blockTextures.allocateFromFiles(std::vector<std::string>{APP_RESOURCE_PATH "/textures/grass_block/top.png"});
      auto grassBlockTextureSide = blockTextures.allocateFromFiles(std::vector<std::string>{APP_RESOURCE_PATH "/textures/grass_block/side.png"});
      auto grassBlockTextureBottom = blockTextures.allocateFromFiles(std::vector<std::string>{APP_RESOURCE_PATH "/textures/grass_block/bottom.png"});
      blockTypes.push_back(std::make_unique<BlockType>("grass_block", BlockTypeAttributes{
        .transparent = false,
      }, std::array<std::shared_ptr<StreamingTexturesPart>, 6>{
        grassBlockTextureSide,
        grassBlockTextureSide,
        grassBlockTextureTop,
        grassBlockTextureBottom,
        grassBlockTextureSide,
        grassBlockTextureSide,
      }));

      auto stoneTexture = blockTextures.allocateFromFiles(std::vector<std::string>{APP_RESOURCE_PATH "/textures/stone/all.png"});
      blockTypes.push_back(std::make_unique<BlockType>("stone", BlockTypeAttributes{
        .transparent = false,
      }, std::array<std::shared_ptr<StreamingTexturesPart>, 6>{
        stoneTexture,
        stoneTexture,
        stoneTexture,
        stoneTexture,
        stoneTexture,
        stoneTexture,
      }));

      auto treeTrunkCrossTexture = blockTextures.allocateFromFiles(std::vector<std::string>{APP_RESOURCE_PATH "/textures/tree_trunk/cross.png"});
      auto treeTrunkSideTexture = blockTextures.allocateFromFiles(std::vector<std::string>{APP_RESOURCE_PATH "/textures/tree_trunk/side.png"});
      blockTypes.push_back(std::make_unique<BlockType>("tree_trunk", BlockTypeAttributes{
        .transparent = false,
      }, std::array<std::shared_ptr<StreamingTexturesPart>, 6>{
        treeTrunkSideTexture,
        treeTrunkSideTexture,
        treeTrunkCrossTexture,
        treeTrunkCrossTexture,
        treeTrunkSideTexture,
        treeTrunkSideTexture,
      }));

      auto treeLeavesTexture = blockTextures.allocateFromFiles(std::vector<std::string>{APP_RESOURCE_PATH "/textures/tree_leaves/all.png"});
      blockTypes.push_back(std::make_unique<BlockType>("tree_leaves", BlockTypeAttributes{
        .transparent = true,
      }, std::array<std::shared_ptr<StreamingTexturesPart>, 6>{
        treeLeavesTexture,
        treeLeavesTexture,
        treeLeavesTexture,
        treeLeavesTexture,
        treeLeavesTexture,
        treeLeavesTexture,
      }));
    }

    // Construct block mesh

    BlocksMap blocksMap(glm::ivec3(-7, 0, -7), glm::ivec3(16, 8, 16));

    for (int i = 0; i < 100; i++) {
      blocksMap[glm::ivec3(-5 + i % 10, 0, -5 + i / 10)].emplace(Block(*blockTypes[1]));
      blocksMap[glm::ivec3(-5 + i % 10, 1, -5 + i / 10)].emplace(Block(*blockTypes[0]));
    }
    for (int i = 0; i < 3; i++) {
      blocksMap[glm::ivec3(2, 2 + i, 2)].emplace(Block(*blockTypes[2]));
    }
    blocksMap[glm::ivec3(2, 5, 2)].emplace(Block(*blockTypes[3]));
    blocksMap[glm::ivec3(1, 4, 2)].emplace(Block(*blockTypes[3]));
    blocksMap[glm::ivec3(3, 4, 2)].emplace(Block(*blockTypes[3]));
    blocksMap[glm::ivec3(2, 4, 1)].emplace(Block(*blockTypes[3]));
    blocksMap[glm::ivec3(2, 4, 3)].emplace(Block(*blockTypes[3]));

    auto blocksMesh = BlocksMesh::buildFromBlocksMap(blocksMap);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, blocksMesh.vertices.size() * sizeof(decltype(blocksMesh.vertices)::value_type), blocksMesh.vertices.data(), GL_STATIC_DRAW);

    GLuint vertexIndexBufferId;
    glGenBuffers(1, &vertexIndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, blocksMesh.vertexIndices.size() * sizeof(decltype(blocksMesh.vertexIndices)::value_type), blocksMesh.vertexIndices.data(), GL_STATIC_DRAW);

    ShaderProgram blocksShaderProgram;
    blocksShaderProgram.loadAndAttachShader(GL_VERTEX_SHADER, "shaders/blocks_vert.glsl");
    blocksShaderProgram.loadAndAttachShader(GL_FRAGMENT_SHADER, "shaders/blocks_frag.glsl");
    blocksShaderProgram.link();

    GLint mvpLocation = glGetUniformLocation(blocksShaderProgram.id(), "MVP");
    GLint atlasCellCountLocation = glGetUniformLocation(blocksShaderProgram.id(), "atlasCellCount");
    GLint texSizeLocation = glGetUniformLocation(blocksShaderProgram.id(), "texSize");
    GLint colorMapLocation = glGetUniformLocation(blocksShaderProgram.id(), "colorMap");

    GLint vPosLocation = glGetAttribLocation(blocksShaderProgram.id(), "vPos");
    GLint vNormLocation = glGetAttribLocation(blocksShaderProgram.id(), "vNorm");
    GLint vTexCoordLocation = glGetAttribLocation(blocksShaderProgram.id(), "vTexCoord");
    GLint vTexPartLocationLocation = glGetAttribLocation(blocksShaderProgram.id(), "vTexPartLocation");

    glEnableVertexAttribArray(vPosLocation);
    glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, x));
    glEnableVertexAttribArray(vNormLocation);
    glVertexAttribPointer(vNormLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, nx));
    glEnableVertexAttribArray(vTexCoordLocation);
    glVertexAttribPointer(vTexCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, u));
    glEnableVertexAttribArray(vTexPartLocationLocation);
    glVertexAttribIPointer(vTexPartLocationLocation, 2, GL_UNSIGNED_INT, sizeof(Vertex), (void*) offsetof(Vertex, tx));

    player.position = glm::vec3(0.f, 3.f, -20.f);
    player.direction(glm::vec3(0.f, 0.f, 1.f));

    while (!glfwWindowShouldClose(window)) {
      float ratio;
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float) height;

      float currentFrameTime = glfwGetTime();
      deltaFrameTime = currentFrameTime - lastFrameTime;
      lastFrameTime = currentFrameTime;

      // Game logic
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.desiredVelocity += player.direction() * 5.f;
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.desiredVelocity -= player.direction() * 5.f;
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.desiredVelocity -= glm::normalize(glm::cross(player.direction(), glm::vec3(0.f, 1.f, 0.f))) * 5.f;
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.desiredVelocity += glm::normalize(glm::cross(player.direction(), glm::vec3(0.f, 1.f, 0.f))) * 5.f;
      if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player.desiredVelocity += glm::vec3(0.f, 1.f, 0.f) * 5.f;
      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        player.desiredVelocity -= glm::vec3(0.f, 1.f, 0.f) * 5.f;
      player.processFrame(deltaFrameTime);

      // Rendering
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      blockTextures.bind();

      glm::mat4 m(1.f);
      glm::mat4 v = glm::lookAt(player.position, player.position + player.direction(), glm::vec3(0.f, 1.f, 0.f));
      glm::mat4 p = glm::perspective(glm::pi<float>() / 4.f, ratio, 0.1f, 100.f);
      glm::mat4 mvp = p * v * m;

      blocksShaderProgram.use();
      glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
      glUniform1i(colorMapLocation, 0);
      glUniform2ui(atlasCellCountLocation, blockTextures.cellCountPerSide(), blockTextures.cellCountPerSide());
      glUniform2ui(texSizeLocation, blockTextures.cellSideLength(), blockTextures.cellSideLength());
      glDrawElements(GL_TRIANGLES, blocksMesh.vertexIndices.size(), GL_UNSIGNED_INT, 0);

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

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
#include "VAO.hpp"
#include "GLBuffer.hpp"
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
    glDepthFunc(GL_LEQUAL);

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

    VAO blocksVao;
    blocksVao.bind();

    GLBuffer blocksVbo(GL_ARRAY_BUFFER);
    blocksVbo.bind();
    blocksVbo.sendData(blocksMesh.vertices, GL_STATIC_DRAW);
    GLBuffer blocksIbo(GL_ELEMENT_ARRAY_BUFFER);
    blocksIbo.bind();
    blocksIbo.sendData(blocksMesh.vertexIndices, GL_STATIC_DRAW);

    ShaderProgram blocksShaderProgram;
    blocksShaderProgram.loadAndAttachShader(GL_VERTEX_SHADER, "shaders/blocks_vert.glsl");
    blocksShaderProgram.loadAndAttachShader(GL_FRAGMENT_SHADER, "shaders/blocks_frag.glsl");
    blocksShaderProgram.link();

    blocksVao.enableAndSetAttribPointer(blocksShaderProgram.getAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), offsetof(BlockVertex, x));
    blocksVao.enableAndSetAttribPointer(blocksShaderProgram.getAttribLocation("vNorm"), 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), offsetof(BlockVertex, nx));
    blocksVao.enableAndSetAttribPointer(blocksShaderProgram.getAttribLocation("vTexCoord"), 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), offsetof(BlockVertex, u));
    blocksVao.enableAndSetAttribIPointer(blocksShaderProgram.getAttribLocation("vTexPartLocation"), 2, GL_UNSIGNED_INT, sizeof(BlockVertex), offsetof(BlockVertex, tx));

    // Make skybox

    VAO skyboxVao;
    skyboxVao.bind();

    GLBuffer skyboxVbo(GL_ARRAY_BUFFER);
    skyboxVbo.bind();
    skyboxVbo.sendData(std::array<glm::vec3, 8>{{
      {-0.5f, -0.5f, -0.5f},
      { 0.5f, -0.5f, -0.5f},
      {-0.5f,  0.5f, -0.5f},
      { 0.5f,  0.5f, -0.5f},
      {-0.5f, -0.5f,  0.5f},
      { 0.5f, -0.5f,  0.5f},
      {-0.5f,  0.5f,  0.5f},
      { 0.5f,  0.5f,  0.5f},
    }}, GL_STATIC_DRAW);
    GLBuffer skyboxIbo(GL_ELEMENT_ARRAY_BUFFER);
    skyboxIbo.bind();
    skyboxIbo.sendData(std::array<GLuint, 36>{
      0, 1, 3, 0, 3, 2, // Z-
      7, 5, 4, 6, 7, 4, // Z+
      6, 4, 0, 2, 6, 0, // X-
      3, 1, 5, 7, 3, 5, // X+
      4, 5, 1, 4, 1, 0, // Y-
      3, 7, 6, 2, 3, 6, // Y+
    }, GL_STATIC_DRAW);

    ShaderProgram skyboxShaderProgram;
    skyboxShaderProgram.loadAndAttachShader(GL_VERTEX_SHADER, "shaders/skybox_vert.glsl");
    skyboxShaderProgram.loadAndAttachShader(GL_FRAGMENT_SHADER, "shaders/skybox_frag.glsl");
    skyboxShaderProgram.link();

    skyboxVao.enableAndSetAttribPointer(skyboxShaderProgram.getAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    player.position = glm::vec3(0.f, 3.f, 0.f);
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

      glm::mat4 m(1.f);
      glm::mat4 v = glm::lookAt(player.position, player.position + player.direction(), glm::vec3(0.f, 1.f, 0.f));
      glm::mat4 p = glm::perspective(glm::pi<float>() / 4.f, ratio, 0.1f, 100.f);
      glm::mat4 mvp = p * v * m;

      // Draw blocks mesh
      blocksVao.bind();
      blockTextures.bind();

      blocksShaderProgram.use();
      blocksShaderProgram.setUniform("MVP", mvp);
      blocksShaderProgram.setUniform("colorMap", 0);
      blocksShaderProgram.setUniform("atlasCellCount", (GLuint) blockTextures.cellCountPerSide(), (GLuint) blockTextures.cellCountPerSide());
      blocksShaderProgram.setUniform("texSize", (GLuint) blockTextures.cellSideLength(), (GLuint) blockTextures.cellSideLength());

      glDrawElements(GL_TRIANGLES, blocksMesh.vertexIndices.size(), GL_UNSIGNED_INT, 0);

      // Draw skybox
      skyboxVao.bind();
      skyboxShaderProgram.use();
      skyboxShaderProgram.setUniform("transMat", p * glm::mat4(glm::mat3(v)));
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

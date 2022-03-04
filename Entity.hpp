#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_
#include <string>
#include <glm/glm.hpp>

class Entity {
private:
  std::string _type;

  glm::vec3 _direction;
  glm::vec2 _rotation; // (pitch, yaw)

public:
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;

  glm::vec3 desiredVelocity;

  Entity(const std::string& type_) : _type(type_) {}
  Entity(std::string&& type_) : _type(type_) {}

  void processFrame(float deltaFrameTime);

  const std::string& type() const { return _type; }
  const glm::vec3& direction() const { return _direction; }
  const glm::vec2& rotation() const { return _rotation; }
  void direction(const glm::vec3& rhs) {
    _direction = rhs;
    _rotation.x = glm::asin(_direction.y);
    _rotation.y = glm::acos(_direction.x / glm::cos(_rotation.x));
  }
  void rotation(const glm::vec2& rhs) {
    _rotation = rhs;
    _direction.x = glm::cos(_rotation.y) * glm::cos(_rotation.x);
    _direction.y = glm::sin(_rotation.x);
    _direction.z = glm::sin(_rotation.y) * glm::cos(_rotation.x);
  }
};

#endif

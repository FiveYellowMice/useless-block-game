#include "Entity.hpp"

template <typename T>
inline constexpr T vecsign(const T& v) {
  return (v == T(0.f)) ? T(0.f) : glm::normalize(v);
}

void Entity::processFrame(float deltaFrameTime) {
  using glm::vec3;
  using glm::dot;
  using glm::length;

  // Try to reach target velocity
  if (length(desiredVelocity - velocity) > 0.001f) {
    acceleration += vecsign(desiredVelocity - velocity) * 8.f;
  }

  // Deceleration caused by friction/drag
  vec3 deceleration = vecsign(velocity) * (2.1f + velocity * velocity * 0.01f);

  velocity += acceleration * deltaFrameTime;
  // To avoid velocity and deceleration oscillating,
  // if applying deceleration would cause velocity to flip direction,
  // treat it as the deceleration has made the entity stop moving
  vec3 velocityWithDeceleration = velocity - deceleration * deltaFrameTime;
  if (dot(vecsign(velocityWithDeceleration), vecsign(velocity)) < 0.f) {
    velocity = vec3(0.f);
  } else {
    velocity = velocityWithDeceleration;
  }
  position += velocity * deltaFrameTime;

  // Reset parameters for the next frame
  acceleration = vec3(0.f);
  desiredVelocity = vec3(0.f);
}

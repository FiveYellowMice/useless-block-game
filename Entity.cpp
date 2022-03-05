#include <glm/ext/scalar_constants.hpp>
#include "Entity.hpp"

// Like normalize(v), but in case the length of v is 0, return 0 instead of NaN
template <typename T>
inline constexpr T vecsign(const T& v) {
  return (glm::length(v) == 0.f) ? T(0.f) : glm::normalize(v);
}

// A functiopn that is close to 1 most of the time, but diminishes to 0 as it approaches 0
// This function exists because when calculating accelerations to reach a desired velocity, multiplying a constant to vecsign(velocity) results in a value as big as the constant, even if the velocity is very close to (but not exactly) 0. Then after adding the resultant acceleration to the velocity, it overshoots to the other side of the desired velocity. On the following frames, this process repeats, velocity always oscillating around the desired velocity but never reaching it. To work around this problem, the aforementioned constant should be multiplied by this function, so that as velocity approaches 0, it will be less likely to overshoot. (It will probably still overshoot thus oscillate if the constant is large or framerate is low, but we don't care about those cases.)
template <typename T>
inline constexpr T diminishTowardsZero(const T& v) {
  return 2 / glm::pi<T>() * glm::atan(v);
}

void Entity::processFrame(float deltaFrameTime) {
  using glm::vec3;
  using glm::length;

  // Try to reach target velocity
  vec3 differenceToDesiredVelocity = desiredVelocity - velocity;
  if (length(differenceToDesiredVelocity) > 0.001f) {
    acceleration += vecsign(differenceToDesiredVelocity) * (
      8.f * diminishTowardsZero(length(differenceToDesiredVelocity))
    );
  }

  // Deceleration caused by friction/drag
  acceleration -= vecsign(velocity) * (
    2.1f * diminishTowardsZero(length(velocity)) +
    0.01f * length(velocity)*length(velocity)
  );

  // Velocity can be so small but not quite 0 but friction becomes 0,
  // so it keeps having this small value, just set it to 0 in thit case
  if (length(velocity) < 0.00001f && length(acceleration) < 0.00001f) {
    velocity = vec3(0.f);
  } else {
    velocity += acceleration * deltaFrameTime;
  }

  position += velocity * deltaFrameTime;

  // Reset parameters for the next frame
  acceleration = vec3(0.f);
  desiredVelocity = vec3(0.f);
}

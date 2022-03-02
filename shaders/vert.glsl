#version 150

uniform mat4 MVP;
uniform uvec2 atlasCellCount;
uniform uvec2 texSize;

in vec3 vPos;
in vec3 vNorm;
in uvec2 vTexPartLocation;
in vec2 vTexCoord;

out vec3 normal;
flat out vec2 uvOffset;
out vec2 uv;

void main() {
  gl_Position = MVP * vec4(vPos, 1.0);
  normal = (MVP * vec4(vNorm, 0.0)).xyz;
  uv = vTexCoord;
  uvOffset = vec2(vTexPartLocation) / atlasCellCount;
}

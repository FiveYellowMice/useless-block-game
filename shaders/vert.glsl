#version 150

uniform mat4 MVP;

attribute vec3 vPos;
attribute vec3 vNorm;
attribute vec2 vTexCoord;

varying vec2 uv;
varying vec3 normal;

void main() {
  gl_Position = MVP * vec4(vPos, 1.0);
  uv = vTexCoord;
  normal = (MVP * vec4(vNorm, 0.0)).xyz;
}

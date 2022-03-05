#version 150

uniform mat4 transMat;

in vec3 vPos;

out vec3 direction;

void main() {
  gl_Position = (transMat * vec4(vPos, 1.0)).xyww;
  direction = vPos;
}

#version 150

uniform sampler2D colorMap;

varying vec2 uv;
varying vec3 normal;

void main() {
  float brightness = max(dot(normal, vec3(0.0, 0.0, -1.0)), 0);
  vec4 color = texture(colorMap, uv);
  if (color.a < 0.5) discard;
  gl_FragColor = vec4(color.rgb * mix(0.2, 1.5, brightness), 1.0);
}

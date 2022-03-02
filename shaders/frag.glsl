#version 150

uniform sampler2D colorMap;
uniform uvec2 atlasCellCount;
uniform uvec2 texSize;

in vec2 uv;
flat in vec2 uvOffset;
in vec3 normal;

void main() {
  float brightness = max(dot(normal, vec3(0.0, 0.0, -1.0)), 0);
  vec2 clampedUv = clamp(uv, 0.5 / texSize, 1.0 - 0.5 / texSize); // avoid texels bleeding from adjacent texture in the atlas
  vec4 color = texture(colorMap, clampedUv / atlasCellCount + uvOffset);
  if (color.a < 0.5) discard;
  gl_FragColor = vec4(color.rgb * mix(0.2, 1.5, brightness), 1.0);
}

#version 150

in vec3 direction;

void main() {
  vec3 skyBlue = vec3(0.844, 0.914, 0.982);
  vec3 skyWhite = vec3(0.95);

  float altitude = dot(normalize(direction), vec3(0, 1, 0));
  vec3 color = mix(skyWhite, skyBlue, vec3(max(altitude, 0)));
  gl_FragColor = vec4(color, 1.0);
}

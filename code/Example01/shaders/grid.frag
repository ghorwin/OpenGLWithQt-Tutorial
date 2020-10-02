#version 330

out vec4 finalColor;  // output: final color value as rgba-value

uniform vec3 gridColor;                // parameter: grid color as rgb triple

void main() {
  finalColor = vec4( gridColor, 1.0 );
}

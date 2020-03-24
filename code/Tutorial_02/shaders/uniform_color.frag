#version 330 core

// fragment shader

in vec4 fragColor;
out vec4 finalColor;  // output: final color value as rgba-value

void main() {
  finalColor = fragColor;
}

#version 330 core

// fragment shader

// test: 'flat' in front of fragColor disables color interpolation
in vec4 fragColor;    // input: interpolated color as rgba-value
out vec4 finalColor;  // output: final color value as rgba-value

void main() {
  finalColor = fragColor;
}

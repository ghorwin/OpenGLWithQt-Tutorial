#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec4 position; // input:  attribute with index '0' with 4 elements per vertex
layout(location = 1) in vec4 color;    // input:  attribute with index '1' with 4 elements (=rgb) per vertex
out vec4 fragColor;                    // output: computed fragmentation color

uniform mat4 worldToView;            // parameter: the camera matrix

void main() {
  // Mind multiplication order for matrixes
  gl_Position = worldToView * vec4(position.xyz, 1.0);
  fragColor = color;
}

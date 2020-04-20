#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec3 position; // input:  attribute with index '0' with 3 elements per vertex

uniform mat4 worldToView;              // parameter: the camera matrix

void main() {
  gl_Position = worldToView * vec4(position, 1.0);
}


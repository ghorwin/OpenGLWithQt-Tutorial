#version 330 core

// vertex shader

// input:  attribute named 'position' with 3 floats per vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 fragColor;

void main() {
  gl_Position = vec4(position, 1.0);
  fragColor = vec4(color, 1.0);
}


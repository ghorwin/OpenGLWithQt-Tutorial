#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec2 position; // input:  attribute with index '0'
                                       //         with 2 floats (x, z coords) per vertex

uniform mat4 worldToView;              // parameter: world to view transformation matrix

void main() {
  gl_Position = worldToView * vec4(position.x, 0.0, position.y, 1.0);
}


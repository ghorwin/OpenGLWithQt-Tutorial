#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec2 position; // input:  attribute with index '0' with 2 elements (x, z coords) per vertex
//out vec4 fragColor;                    // output: computed fragmentation color


uniform mat4 worldToView;              // parameter: the camera matrix

void main() {
  // Mind multiplication order for matrixes
  gl_Position = worldToView * vec4(position.x, 0.0, position.y, 1.0);
}



#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec3 position; // input:  attribute with index '0' with 3 elements per vertex
layout(location = 1) in vec3 color;    // input:  attribute with index '1' with 3 elements (=rgb) per vertex
out vec4 fragColor;                    // output: computed fragmentation color

uniform mat4 modelToWorld;             // parameter: the transformation matrix
uniform mat4 worldToCamera;            // parameter: the camera matrix
uniform mat4 cameraToView;             // parameter: the projection matrix

void main() {
  // Mind multiplication order for matrixes
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
  fragColor = vec4(color, 1.0);
}



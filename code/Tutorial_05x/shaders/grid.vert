#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec2 position; // input:  attribute with index '0' with 2 elements (x, z coords) per vertex
out vec4 fragColor;                    // output: computed fragmentation color
out float fragDepth;                   // also output: depth value computed by

const float  FARPLANE = 100;          // might be a uniform to the shader

uniform mat4 modelToWorld;             // parameter: the transformation matrix
uniform mat4 worldToCamera;            // parameter: the camera matrix
uniform mat4 cameraToView;             // parameter: the projection matrix
uniform vec3 gridColor;                // parameter: grid color as rgb triple

void main() {
  // Mind multiplication order for matrixes
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position.x, 0.0, position.y, 1.0);
  fragDepth = gl_Position.z / FARPLANE; // do not divide by w
  fragColor = vec4(gridColor, 1.0);
}



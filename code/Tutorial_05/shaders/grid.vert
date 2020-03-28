#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec2 position; // input:  attribute with index '0' with 2 elements (x, z coords) per vertex
out vec4 fragColor;                    // output: computed fragmentation color

const float FARPLANE = 50;             // threshold
float fragDepth;                       // normalized depth value

uniform mat4 worldToView;              // parameter: the camera matrix
uniform vec3 gridColor;                // parameter: grid color as rgb triple
uniform vec3 backColor;                // parameter: background color as rgb triple

void main() {
  // Mind multiplication order for matrixes
  gl_Position = worldToView * vec4(position.x, 0.0, position.y, 1.0);
  fragDepth = min(1, gl_Position.z / FARPLANE); // do not divide by w
  fragColor = vec4(gridColor, 1.0); //vec4( mix(gridColor, backColor, fragDepth), 1.0);
}



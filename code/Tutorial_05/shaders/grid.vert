#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec2 position; // input:  attribute with index '0' with 2 elements (x, z coords) per vertex
out vec4 fragColor;                    // output: computed fragmentation color

const float  FARPLANE = 100;          // might be a uniform to the shader

uniform mat4 worldToView;              // parameter: the world to view transformation matrix
uniform vec3 gridColor;                // parameter: grid color as rgb triple
uniform vec3 backgroundColor = vec3(0.1, 0.1, 0.2);  // parameter: grid color as rgb triple

float fragDepth;

void main() {
  gl_Position = worldToView * vec4(position.x, 0.0, position.y, 1.0);
  fragDepth = min(1, gl_Position.z / FARPLANE); // do not divide by w
  // mix color between grid color and background color, depending on normalized depth
  //fragColor = vec4( mix(gridColor, backgroundColor, fragDepth), 1.0);
  fragColor = vec4(gridColor, 1.0);
}

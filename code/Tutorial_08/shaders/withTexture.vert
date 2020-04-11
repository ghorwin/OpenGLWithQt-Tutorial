#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec3 position;   // input:  attribute with index '0' with 3 elements per vertex
layout(location = 1) in vec3 color;      // input:  attribute with index '1' with 3 elements (=rgb) per vertex
layout(location = 2) in vec2 texcoords;  // input:  attribute with index '2' with 2 elements per vertex
layout(location = 3) in float texnr;       // input:  attribute with index '3' - uint per vertex
out vec4 fragColor;                      // output: computed fragmentation color
out vec2 texCoord;                       // output: computed texture coordinates
flat out float texID;                      // output: texture ID - mind the 'flat' attribute!
uniform mat4 worldToView;            // parameter: the camera matrix

void main() {
  // Mind multiplication order for matrixes
  gl_Position = worldToView * vec4(position, 1.0);
  fragColor = vec4(color, 1.0);
  texCoord = texcoords;
  texID = texnr;
}



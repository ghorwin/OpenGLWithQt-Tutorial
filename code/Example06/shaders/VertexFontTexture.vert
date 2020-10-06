#version 330

// GLSL version 3.3
// vertex shader

layout(location = 0) in vec3 position;    // input:  attribute with index '0' with 3 elements per vertex
layout(location = 1) in vec2 texinfo;     // input:  attribute with index '1' with 2 elements per vertex (texi, texj)

out vec2 texCoord;                        // output: computed texture coordinates
//flat out float texID;                     // output: texture ID - mind the 'flat' attribute (disables interpolation)!

uniform mat4 worldToView;                 // parameter: the camera matrix

void main() {
  gl_Position = worldToView * vec4(position, 1.0);
  texCoord = texinfo;
//  texID = texinfo.z;
}



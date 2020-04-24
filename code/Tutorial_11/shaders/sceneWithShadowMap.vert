#version 330 core
layout(location = 0) in vec3 position;   // input:  attribute with index '0' with 3 elements per vertex
layout(location = 1) in vec3 color;      // input:  attribute with index '1' with 3 elements (=rgb) per vertex
layout(location = 2) in vec3 normal;     // input:  attribute with index '2' with 3 elements per vertex

out VS_OUT {
  vec3 FragPos;            // position of fragment in world coordinates
  vec4 FragPosLightSpace;  // position of fragment in light-space coordinates
  vec3 FragNormal;         // normal vector of fragment
  vec3 FragColor;          // color of fragment
} vs_out;

uniform mat4 worldToView;                     // parameter: the camera matrix
uniform mat4 lightSpaceMatrix;                // parameter: the light space matrix

void main()
{
  vs_out.FragPos = position;
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
  vs_out.FragNormal = normal;
  vs_out.FragColor = color;
  gl_Position = worldToView * vec4(vs_out.FragPos, 1.0);
}


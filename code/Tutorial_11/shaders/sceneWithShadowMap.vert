#version 330 core
layout(location = 0) in vec3 position; // input:  attribute with index '0' with 3 elements per vertex
layout(location = 1) in vec3 color;    // input:  attribute with index '1' with 3 elements (=rgb) per vertex

out VS_OUT {
  vec3 FragPos;
  vec4 FragPosLightSpace;
  vec3 FragColor;
} vs_out;

uniform mat4 worldToView;                     // parameter: the camera matrix
uniform mat4 lightSpaceMatrix;                // parameter: the light space matrix

void main()
{
  vs_out.FragPos = position;
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
  vs_out.FragColor = color;
  gl_Position = worldToView * vec4(vs_out.FragPos, 1.0);
}


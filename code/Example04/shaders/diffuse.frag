#version 330 core

// fragment shader with diffuse lighting

in vec3 fragColor;         // input: interpolated color as rgba-value
in vec3 fragNormal;        // input: normal vector of fragment
in vec3 fragPos;           // input: fragment position (world coords)

out vec4 finalColor;       // output: final color value as rgba-value

uniform vec3 lightPos;     // parameter: light position as vec3 (world coords)
uniform vec3 lightColor;   // parameter: light color as rgb

void main() {
  // ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse 
  vec3 norm = normalize(fragNormal);
  vec3 lightDir = normalize(fragPos - lightPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
            
  vec3 result = (ambient + diffuse) * fragColor;
  
  // to test correct passing of lightPos
  // finalColor = vec4(normalize(lightPos), 1.0);  
    
  // to test correct passing of lightColor
  // finalColor = vec4(lightColor, 1.0);  
    
  // to test fragment position component DISCUSS
  // finalColor = vec4(normalize(fragPos), 1.0);  
  
  // to test fragment color
  // finalColor = vec4(fragColor, 1.0);  
  
  // to test normal component
  // finalColor = vec4(norm, 1.0);  
  
  finalColor = vec4(result, 1.0);
}

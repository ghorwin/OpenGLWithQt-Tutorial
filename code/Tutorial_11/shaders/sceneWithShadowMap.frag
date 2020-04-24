#version 330 core
out vec4 FinalColor;

in VS_OUT {
	vec3 FragPos;            // position of fragment in world coordinates
	vec4 FragPosLightSpace;  // position of fragment in light-space coordinates
	vec3 FragNormal;         // normal vector of fragment
	vec3 FragColor;          // color of fragment
} fs_in;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;
  // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  float closestDepth = texture(shadowMap, projCoords.xy).r;
  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  // check whether current frag pos is in shadow
//  float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
  float bias = 0.001;
  float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

  return shadow;
}

void main()
{
  vec3 color = fs_in.FragColor;
  vec3 normal = normalize(fs_in.FragNormal);
  vec3 lightColor = vec3(1.0);
  // ambient
  vec3 ambient = 0.15 * color;
  // diffuse
  vec3 lightDir = normalize(lightPos - fs_in.FragPos);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * lightColor;
  // specular
  vec3 viewDir = normalize(viewPos - fs_in.FragPos);
  float spec = 0.0;
  vec3 halfwayDir = normalize(lightDir + viewDir);
  spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
  vec3 specular = spec * lightColor;
  // calculate shadow: 1 - in light, 0 - dark
  float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
  // compose final light value - mind that this can lead to a brighter color than the original color
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
  FinalColor = vec4(lighting, 1.0);
}


#version 330

out vec4 fColor;

uniform vec3 gridColor;                // parameter: grid color as rgb triple
uniform vec3 backColor;                // parameter: background color as rgb triple
const float FARPLANE = 150;            // threshold

void main() {
  float distanceFromCamera = 1.0 - (gl_FragCoord.z / gl_FragCoord.w) / FARPLANE;
  distanceFromCamera = max(0, min(1, distanceFromCamera)); // clip to valid value range
  fColor = vec4( mix(backColor, gridColor, distanceFromCamera), 1.0 );
}

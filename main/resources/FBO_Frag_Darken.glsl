#version 120

varying vec2 UV;
uniform sampler2D uTex;
uniform float intensity;

float tunnelVision() {
    float darkening = length(0.5 - UV) * intensity;
    return darkening;
}

void main() {
    vec4 currColor = texture2D(uTex, UV);
    gl_FragColor = mix(currColor, vec4(0, 0, 0, 1), tunnelVision());
}

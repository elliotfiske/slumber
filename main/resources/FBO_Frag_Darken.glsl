#version 120

uniform float time;
const float PI = 3.1415926535897932;

varying vec2 UV;
uniform sampler2D uTex;
uniform float intensity;
uniform float darknessMod; // alters the color of darkness when zoomed

float tunnelVision() {
    float darkening = length(0.5 - UV) * intensity;
    return darkening;
}

vec2 woozyUV() {
    vec2 modifiedUV = UV;
    
    float dx = sin(UV.x * 3 * PI) * cos(time / 18.2) * 0.046;
    float dy = sin(UV.y * 4 * PI) * sin(time / 18.2) * 0.046;
    
    float centerFactor = distance(vec2(0.5, 0.5), UV);
    
    dx *= centerFactor;
    dy *= centerFactor;
    
    modifiedUV.x += dx;
    modifiedUV.y += dy;
    
    return modifiedUV;
}

void main() {
    vec2 distortedUV = woozyUV();
    
    vec4 currColor = texture2D(uTex, distortedUV);
    
    float centerDist = distance(vec2(0.5, 0.5), UV);
    if (centerDist < 0.002) {
        gl_FragColor = vec4(0.6, 0.6, 0.6, 1);
    }
    else {
        gl_FragColor = mix(currColor, vec4(0.2 * darknessMod, 0, 0, 1), tunnelVision());
    }
}

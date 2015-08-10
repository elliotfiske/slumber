#version 120


uniform float time;
uniform vec2 resolution;

varying vec2 UV;
uniform sampler2D uTex;
uniform float intensity;
uniform float darknessMod; // used in the darkening shader

void main() {
    vec2 modifiedUV = UV;
    
    float dx = sin((UV.x)*2.6 + time * 0.1)*0.05;
    float dy = cos((UV.y)*2.6 + time * 0.1)*0.05;
    
    float centerFactor = distance(vec2(0.5, 0.5), UV);
    
    dx *= centerFactor;
    dy *= centerFactor;
    
    modifiedUV.x += dx;
    modifiedUV.y += dy;
    
    gl_FragColor = texture2D(uTex, modifiedUV);
}

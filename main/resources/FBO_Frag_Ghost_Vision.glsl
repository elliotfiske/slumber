#version 120

const float PI = 3.1415926535897932;
const float frequency = 6.0;

const float speed = 0.1;
const float emboss = 0.70;
const int steps = 8;
const int angle = 7;

const float delta = 200.;

uniform float time;

varying vec2 UV;
uniform sampler2D uTex;
uniform float intensity;

void main() {
    vec2 modifiedUV = UV;
    vec2 c2 = UV;
    
    float dx = sin(UV.x * 6 * PI) * cos(time * 2.2) * 0.01;
    float dy = sin(UV.y * 6 * PI) * sin(time * 2.2) * 0.01;
    
    float dx2 = sin(UV.x * 12 * PI) * cos(time * 2.0) * 0.02;
    float dy2 = sin(UV.y * 12 * PI) * sin(time * 2.0) * 0.02;
    
    float dx3 = sin(UV.x * 18 * PI) * cos(time * 1.6) * 0.007;
    float dy3 = sin(UV.y * 18 * PI) * sin(time * 1.6) * 0.007;
    
    dx += dx2 + dy3;
    dy += dy2 + dy3;
    
    float centerFactor = pow(distance(vec2(0.5, 0.5), UV) * 1.15, 3.5);
    
    dx *= centerFactor;
    dy *= centerFactor;
    
    modifiedUV.x += dx;
    modifiedUV.y += dy;
    
    gl_FragColor = texture2D(uTex, modifiedUV);
}
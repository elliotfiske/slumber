#version 120

const float PI = 3.1415926535897932;
const float frequency = 6.0;

const float speed = 0.1;

uniform float time;

varying vec2 UV;
uniform sampler2D uTex;
uniform float intensity;
uniform float darknessMod; // used in the darkening shader

vec2 wobble() {
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
    
    float centerFactor = pow(distance(vec2(0.5, 0.5), UV) * 1.25, 3.5);
    
    dx *= centerFactor;
    dy *= centerFactor;
    
    modifiedUV.x += dx;
    modifiedUV.y += dy;
    
    return modifiedUV;
}


const float sampleDist = 0.007;

void main() {
    float samples[10];
    samples[0] =  0.01;
    samples[1] =  0.015;
    samples[2] =  0.02;
    samples[3] =  0.03;
    samples[4] =  0.035;
    samples[5] =  0.039;
    samples[6] =  0.043;
    samples[7] =  0.0455;
    samples[8] =  0.0465;
    samples[9] =  0.047;
    
    vec2 wobbleUV = wobble();
    
    vec2 dir = 0.5 - UV;
    float dist = distance(vec2(0.5, 0.5), UV) * 16.8;
    dist = pow(dist, 2.0);
    dir = dir*dist;
    
    vec4 color = texture2D(uTex, wobbleUV);
    vec4 sum = color;
    
    for (int i = 0; i < 10; i++)
        sum += texture2D(uTex, wobbleUV + dir * samples[i] * sampleDist);
    
    sum *= 1.0/11.0;
    float t = dist * intensity;
    t = clamp(t, 0.0, 1.0);
    
    gl_FragColor = mix(color, sum, t);
}

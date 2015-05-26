#version 120

varying vec2 UV;
uniform sampler2D uTex;
uniform float intensity;

const float sampleDist = 0.02;

void main() {
    float samples[10];
    samples[0] =  0.01;
    samples[1] =  0.02;
    samples[2] =  0.03;
    samples[3] =  0.04;
    samples[4] =  0.05;
    samples[5] =  0.06;
    samples[6] =  0.07;
    samples[7] =  0.08;
    samples[8] =  0.09;
    samples[9] =  0.10;
    
    vec2 dir = 0.5 - UV;
    float dist = distance(vec2(0.5, 0.5), UV) * 1.3;
    dist = pow(dist, 2.0);
    dir = dir/dist;
    
    vec4 color = texture2D(uTex, UV);
    vec4 sum = color;
    
    for (int i = 0; i < 10; i++)
        sum += texture2D(uTex, UV + dir * samples[i] * sampleDist);
    
    sum *= 1.0/11.0;
    float t = dist * intensity;
    t = clamp(t, 0.0, 1.0);
    
    gl_FragColor = mix(color, sum, t);
}
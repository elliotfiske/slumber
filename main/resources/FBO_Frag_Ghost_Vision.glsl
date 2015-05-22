#version 120

const float PI = 3.1415926535897932;
const float frequency = 6.0;

const float speed = 0.1;
const float emboss = 0.70;
const int steps = 8;
const int angle = 7;

const float delta = 200.;

uniform float time;
uniform vec2 resolution;

varying vec2 UV;
uniform sampler2D uTex;
uniform float intensity;

float color(vec2 coord) {
    float delta_theta = 2.0 * PI / float(angle);
    float col = 0.0;
    float theta = 0.0;
    
    for (int i = 0; i < steps; i++) {
        vec2 adjc = coord;
        theta = delta_theta*float(i);
        adjc.x += cos(theta)*time*speed;
        adjc.y -= sin(theta)*time*speed;
        col = col + cos( (adjc.x*cos(theta) - adjc.y*sin(theta))*frequency)*intensity;
    }
    
    return cos(col);
}

void main() {
    vec2 modifiedUV = UV;
    vec2 c2 = UV;
//    float cc1 = color(c1);
//    
//    c2.x += resolution.x/delta;
//    float dx = emboss*(cc1-color(c2))/delta;
//    
//    c2.x = UV.x;
//    c2.y += resolution.y/delta;
//    float dy = emboss*(cc1-color(c2))/delta;
//    
//    vec2 center = vec2(resolution.x / 2.0, resolution.y / 2.0);
//    float distToCenter = distance(center, gl_FragCoord.xy);
//    dx *= distToCenter / 600.0;
//    dy *= distToCenter / 600.0;
//    
//    c1.x += dx*2.;
//    c1.y = -(c1.y+dy*2.0);
    float dx = sin(UV.x * 6 * PI) * cos(time * 0.5) * 0.02;
    float dy = sin(UV.y * 6 * PI) * sin(time * 0.5) * 0.02;
    
    float centerFactor = pow(distance(vec2(0.5, 0.5), UV), 2.0);
    
    dx *= centerFactor;
    dy *= centerFactor;
    
    modifiedUV.x += dx;
    modifiedUV.y += dy;
    
    vec4 currColor = texture2D(uTex, modifiedUV);
    
    // Make everything lighter and blue
    currColor.z = (currColor.x + currColor.y + currColor.z) / 3.0 * 1.9;
    currColor.x /= 2.0;
    currColor.y /= 1.6;
    
    currColor += 0.05;
    
    gl_FragColor = currColor;
}
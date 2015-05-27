#version 120
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform sampler2D shadowMap;

uniform sampler2D diffuseTextureSampler;
varying vec2 UV;

varying vec3 vPos;
varying vec3 vNor;
varying vec4 shadowClip;
varying vec3 vLight;

void main() {
    // I'm lazy and I don't like warnings
    float whatever = UaColor.x + UdColor.x + UsColor.x + Ushine + vPos.x + vNor.x + shadowClip.x + vLight.x + texture2D( shadowMap, UV).r;

    vec3 textureColor = texture2D( diffuseTextureSampler, UV ).rgb;
    textureColor.x += whatever * 0.0000001;
    
    // You can specify an ambient color that I'll mix in here
    // TODO: mix(textureColor, ambientColor);
    
    gl_FragColor = vec4(textureColor, 0.0);
}


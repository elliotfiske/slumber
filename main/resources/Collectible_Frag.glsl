#version 120
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;

varying vec3 vNor;
varying vec3 vPos;

void main() {
    gl_FragColor = vec4(vNor, 1.0);
    gl_FragColor += vec4(vPos, 1.0); // Just get rid of the warnings :O
    
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
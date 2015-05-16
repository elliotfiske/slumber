#version 120
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;

varying vec3 vNor;

void main() {
    gl_FragColor = vec4(vNor, 0.0);
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
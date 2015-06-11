#version 120
attribute vec4 aPosition;
attribute vec2 aUV;

varying vec2 UV;

void main() {
    UV = aUV;
    UV.y *= -1.0;
    
    gl_Position = aPosition;
}

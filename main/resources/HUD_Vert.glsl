#version 120
attribute vec4 aPosition;
attribute vec2 aUV;

uniform vec2 screenSize;
uniform vec2 imageSize;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying vec3 vPos;

varying vec2 UV;

void main() {
    mat4 MV = uViewMatrix * uModelMatrix;
    vec4 pos_cam = MV * aPosition;
    
    gl_Position = uProjMatrix * pos_cam;
    
    if (imageSize.x != 0.) {
        gl_Position.x = gl_Position.x * imageSize.x / screenSize.x;
        gl_Position.y = gl_Position.y * imageSize.y / screenSize.y;
    }
    
    UV = aUV;
    UV.y *= -1.0;
}

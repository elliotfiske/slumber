#version 120
uniform sampler2D uTex;
varying vec2 UV;

void main() {
    vec4 textureColor = texture2D( uTex, UV );

    gl_FragColor = textureColor;
}
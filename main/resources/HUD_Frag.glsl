#version 120
uniform sampler2D diffuseTextureSampler;
varying vec2 UV;

void main() {
    vec4 textureColor = texture2D( diffuseTextureSampler, UV );

    gl_FragColor = textureColor;
}


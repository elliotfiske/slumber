#version 120

uniform float percentShown;
uniform sampler2D diffuseTextureSampler;
varying vec2 UV;

void main() {
    // Blinn-Phong

	float level = -percentShown / 100.0;

	vec4 color;

	if (UV.y < level) {
		color = vec4(0.0, 0.0, 0.0, 0.5);
	}
	else {
		color = texture2D( diffuseTextureSampler, UV );
	}

    gl_FragColor = color;
}


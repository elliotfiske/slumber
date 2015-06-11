#version 120

uniform float percentShown;
uniform sampler2D diffuseTextureSampler;
varying vec2 UV;

uniform float fadiness;       // Denotes how much we should fade in the HUD
uniform float fadeTopPercentage;    // Denotes what part of the text to fade
uniform float fadeBottomPercentage;

void main() {
    // Blinn-Phong

	float level = -(0.2 + percentShown / 100.0 * .52);
	// .20

	vec4 color;

	if (UV.y < level) {
        discard;
	}
	else {
		color = texture2D( diffuseTextureSampler, UV );
	}

    gl_FragColor = color;
}


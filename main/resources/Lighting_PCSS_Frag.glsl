#version 120
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform sampler2D shadowMap;
uniform float attenFactor;

uniform sampler2D diffuseTextureSampler;
varying vec2 UV;

varying vec3 vPos;
varying vec3 vNor;
varying vec4 shadowClip;
varying vec4 highlightCoords;
varying vec3 vLight;

float searchBlocker(vec2 uv, vec4 lightPos, float searchWidth);
float estimatePenumbra(vec2 uv, vec4 lightPos, float blocker, float lightSize);
float PCF(vec2 uv, vec4 lightPos, float filterWidth);

void main() {

    // Blinn-Phong
    
    float distToCam = length(vPos);
    float distToLight = length(vLight - vPos);
    vec3 n = normalize(vNor);
    vec3 l = (vLight - vPos) / distToLight;
    vec3 e = -vPos / distToCam;
    vec3 h = normalize(l + e);
    float cd = max(0.0, dot(n, l));
    float cs = pow(max(0.0, dot(n, h)), Ushine);
    float attenuation = 1.0 / (1.0 + attenFactor * distToLight + attenFactor * distToLight * distToLight);

    vec3 textureColor = texture2D( diffuseTextureSampler, UV ).rgb;
    textureColor += UdColor;
    
    vec3 lAmbientColor  = UaColor * attenuation;
    vec3 lDiffuseColor  = cd * textureColor * attenuation;
    vec3 lSpecularColor = cs * UsColor * attenuation;

    // Shadowing

    vec4 shadowCoords = shadowClip;
    shadowCoords = shadowCoords / shadowCoords.w;
    // Go from [-1,1] to [0,1]
    shadowCoords.xyz = 0.5 * shadowCoords.xyz + 0.5;

	float visibility = 1.0;

	// Only calculate if the fragment is within the shadowmap's bounds
	if(shadowCoords.w > 0.0 &&
        shadowCoords.x > 0.0 && shadowCoords.x < 1.0 &&
        shadowCoords.y > 0.0 && shadowCoords.y < 1.0 &&
        shadowCoords.z > 0.0 && shadowCoords.z < 1.0) {
		// STEP 1: Blocker estimate
		float blocker = searchBlocker(shadowCoords.xy, shadowCoords, 1.0);
		// STEP 2: Penumbra size estimation
		float penumbra = estimatePenumbra(shadowCoords.xy, shadowCoords, blocker, 0.005);
	 
		if(penumbra > 0.01) penumbra = 0.01;
	 
		visibility = PCF(shadowCoords.xy, shadowCoords, penumbra);

	}

    gl_FragColor = vec4(lAmbientColor + visibility * (lDiffuseColor + lSpecularColor), 1.0);
}

// Search near the point for blockers
float searchBlocker(vec2 uv, vec4 lightPos, float searchWidth)
{
	float stepSize = 2.0 * searchWidth/16.0;
 
	// Starting point uv coords for search
	uv = uv - vec2(searchWidth, searchWidth);
	vec2 uvClipped = vec2(max(uv.x, 0.0), max(uv.y, 0.0));
 
	float blockerSum = 0.0;
	float blockerCount = 0.0;

	// iterate through search region and add up depth values
    for(int i = 0; i < 16; i += 1) {
		for(int j = 0; j < 16; j += 1) {
			float distToLight = texture2D(shadowMap, uvClipped + vec2(i * stepSize, j * stepSize)).z;

			// found a blocker
			if(distToLight < lightPos.z) {
				blockerSum += distToLight;
				blockerCount += 1.0;
			}
		}
	}
 
	float result = blockerSum/blockerCount;
	return result;
}

// Estimates the size of the penumbra
float estimatePenumbra(vec2 uv, vec4 lightPos, float blocker, float lightSize)
{ 
       // estimate penumbra using parallel planes approximation
       return (lightPos.z - blocker) * lightSize / blocker;
}

// Percentage Closer Filtering
float PCF(vec2 uv, vec4 lightPos, float filterWidth)
{
	float stepSize = 2.0 * filterWidth/16.0;

	uv = uv - vec2(filterWidth, filterWidth);
	vec2 uvClipped = vec2(max(uv.x, 0.0), max(uv.y, 0.0));

	float sum = 0.0;

	// now iterate through the kernel and filter the values
	for(int i = 0; i < 16; i += 1)
	{
		for(int j = 0; j < 16; j += 1)
		{
			float distToLight = texture2D(shadowMap, uvClipped + vec2(i*stepSize, j*stepSize)).z;

			// test if the depth in the shadow map is closer than the eye-view point
			float shadowed = lightPos.z < distToLight ? 1.0 : 0.0;

			sum += shadowed;
		}
	}

	// return average of the samples
	return sum / (16.0 * 16.0);
}

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

vec2 poissonDisk[] = vec2[](
    vec2( -0.942011224, -0.3990122112 ),
    vec2( 0.945581209, -0.712890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.293877120 ),
    vec2( -0.91588581, 0.45771432 ),
    vec2( -0.81544232, -0.879124124 ),
    vec2( -0.38277543, 0.2712712845 ),
    vec2( 0.97484398, 0.751248379 ),
    vec2( 0.44323325, -0.97511554 ),
    vec2( 0.53742981, -0.47373420 ),
    vec2( -0.2124912911, -0.41893023 ),
    vec2( 0.79197514, 0.19090188 ),
    vec2( -0.24188840, 0.997012507 ),
    vec2( -0.81409955, 0.91437590 ),
    vec2( 0.199841212, 0.7812413127 ),
    vec2( 0.143831121, -0.14100790 )
);

float FindBlocker(vec2 uv, vec4 lightPos, float searchWidth);
float EstimatePenumbra(vec2 uv, vec4 lightPos, float blocker, float lightSize);
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
    
    float bias = 0.000001;
    float blur = 0.0001;
    float visibility = 1.0;

    ///////////////////////////////////////////////////
	// STEP 1: Blocker estimate
	float blocker = FindBlocker(shadowCoords.xy, shadowCoords, 1.0);
 
	///////////////////////////////////////////////////
	// STEP 2: Penumbra size estimation
	float penumbra = EstimatePenumbra(shadowCoords.xy, shadowCoords, blocker, 0.01);
 
	if(penumbra > 0.01)
		penumbra = 0.01;
 
	visibility = PCF(shadowCoords.xy, shadowCoords, penumbra);
 
	// If no blocker, return 1.0 since the point isn't in shadow
	if(blocker == 0.0) visibility = 1.0;

    gl_FragColor = vec4(lAmbientColor + visibility * (lDiffuseColor + lSpecularColor), 1.0);
}

/*
 * Search for potential blockers
 */
float FindBlocker(vec2 uv, vec4 lightPos, float searchWidth)
{
	float stepSize = 2.0 * searchWidth/12.0;
 
	// Starting point uv coords for search
	uv = uv - vec2(searchWidth, searchWidth);
 
	float blockerSum = 0.0;
	float blockerCount = 0.0;
 
	// iterate through search region and add up depth values
    for(int i = 0; i < 12; i += 1) {
		for(int j = 0; j < 12; j += 1) {
			float shadMapDepth = texture2D(shadowMap, uv + vec2(i * stepSize, j * stepSize)).z;

			// found a blocker
			if(shadMapDepth < lightPos.z) {
				blockerSum += shadMapDepth;
				blockerCount += 1.0;
			}
		}
	}
 
	float result = blockerSum/blockerCount; // FIXME
	return result;
}

/*
 * Function to estimate the shadow penumbra size.
 */
float EstimatePenumbra(vec2 uv, vec4 lightPos, float blocker, float lightSize)
{ 
       // estimate penumbra using parallel planes approximation
       float penumbra = (lightPos.z - blocker) * lightSize/blocker;
 
       return penumbra;
}

/*
 * Percentage Closer Filtering with customizable filter kernel size and sample amount.
 */
float PCF(vec2 uv, vec4 lightPos, float filterWidth)
{
	float stepSize = 2.0 * filterWidth/12.0;

	uv = uv - vec2(filterWidth, filterWidth); // FIXME: why not (uv - filterWidth) ?

	float sum = 0.0;

	// now iterate through the kernel and filter the values
	for(int i = 0; i < 12; i += 1) // FIXME
	{
		for(int j = 0; j < 12; j += 1) // FIXME
		{
			// get depth at current texel of the shadow map
			float shadMapDepth = texture2D(shadowMap, uv + vec2(i*stepSize, j*stepSize)).z;

			// test if the depth in the shadow map is closer than the eye-view point
			float shad = lightPos.z < shadMapDepth ? 1.0 : 0.0;

			// accumulate result
			sum += shad;
		}
	}

	// return average of the samples
	return sum/(12.0 * 12.0);
}

#version 120

varying vec2 UV;
uniform sampler2D tex;

float tunnelVision() {
    float darkening = length(0.5 - UV) * 2;
    return darkening;
}

void main(){
    vec4 currColor = texture2D(tex, UV);
//    
//    currColor.x -= 0.9;
//    currColor.y -= 0.9;
//    currColor.z -= 0.9;
//    
//    currColor.x *= 4;
//    currColor.y *= 4;
//    currColor.z *= 4;
    gl_FragColor = mix(currColor, vec4(0, 0, 0, 1), tunnelVision());
}

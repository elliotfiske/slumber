#version 120

varying vec2 UV;
uniform sampler2D tex;

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
    
    gl_FragColor = currColor;
}

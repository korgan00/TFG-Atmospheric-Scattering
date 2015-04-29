
#version 430 core

in  vec4 color;

//out vec4 fColor;

layout(location = 0) out float fragmentdepth;

void main() {
    //fColor = color;

	fragmentdepth = gl_FragCoord.z;
}

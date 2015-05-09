#version 400 core

in  vec4 color;
uniform sampler2D texture_diffuse;

out vec4 fcolor;

void main(void) {

    //fcolor = color;
	fcolor = texture(texture_diffuse, color.xy);// *color.z;
}
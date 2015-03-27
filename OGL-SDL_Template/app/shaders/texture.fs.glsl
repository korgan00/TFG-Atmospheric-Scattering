#version 430

in vec4 vs_fs_color;
uniform sampler2D texture_diffuse;
//uniform sampler2D texture2;

layout (location = 0) out vec4 color;

void main(void) {

    //color = vs_fs_color;
	color = texture(texture_diffuse, vs_fs_color.xy) * vs_fs_color.z;
}

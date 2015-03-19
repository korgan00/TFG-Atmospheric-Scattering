#version 330

uniform mat4 projection_matrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texPosition;

out vec4 vs_fs_color;

void main(void) {
	//vs_fs_color = vec4(((position + vec3(1, 1, 1)) / 2), 1);
	float i = position.y / 10;
	i = i*i;
	if (i > 1) i = 1;
	//vs_fs_color = vec4(i, 0, 0, 1);*/

	vs_fs_color = vec4(texPosition, i, 1);
	gl_Position = projection_matrix * vec4(position, 1);
}

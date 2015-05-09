#version 430

uniform mat4 projection_matrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texPosition;

out vec4 vs_fs_color;

void main(void) {

	float i = position.y / 10.0f;
	i = i*i;
	if (i > 1.0f) i = 1.0f;


	vs_fs_color = vec4(texPosition, i, 1.0f);
	gl_Position = projection_matrix * vec4(position, 1.0f);
}

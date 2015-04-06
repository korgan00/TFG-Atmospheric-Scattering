#version 430

//uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

out vec4 vs_fs_color;
out vec3 obj;

void main(void) {
	//////vec3 sun = vec3(1000, 1000, 1000);
	//vs_fs_color = vec4(((position + vec3(1, 1, 1)) / 2), 1);
	//float i = position.y / 10;
	//i = i*i;
	//if (i > 1) i = 1;
	//vs_fs_color = vec4(i, 0, 0, 1);*/

	vec3 pos = vPos * 10;
	//pos.y -= 5;
	vs_fs_color = vec4(vTex, 0, 1);
	gl_Position = projection_matrix * (model_matrix * vec4(pos, 1));
	obj = pos;
}

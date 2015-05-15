
#version 430 core

uniform mat4 projection_matrix;
uniform mat4 model_matrix;

uniform vec3 cam;

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

out vec4 color;

void main() {
	vec4 pos = (model_matrix * vec4(vPos, 1));

	color = vec4((vPos+1)/2, 1);
	gl_Position = projection_matrix * pos;
	//obj = pos.xyz;
}

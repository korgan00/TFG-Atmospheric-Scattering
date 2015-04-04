
#version 430 core

uniform mat4 projection_matrix;

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

out vec4 color;

void
main()
{
	color = vec4(vTex, 0.0f, 1.0f);
	gl_Position = projection_matrix * vec4(vPos, 1.0f);
}

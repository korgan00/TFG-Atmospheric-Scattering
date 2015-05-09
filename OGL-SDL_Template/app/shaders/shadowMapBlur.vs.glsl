
#version 430 core

layout(location = 0) in vec2 vPos;
layout(location = 1) in vec2 vTex;

out vec2 texPos;

void main() {
	texPos = vTex;
	gl_Position = vec4(vPos, 0.0f, 1.0f);
}

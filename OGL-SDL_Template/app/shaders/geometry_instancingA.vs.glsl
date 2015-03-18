#version 330

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

// per instance attribute
layout (location = 2) in vec4 translation;

out vec4 vs_fs_color;

void main(void)
{
    mat4 trans = mat4(   1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         translation); //*/ 10, 0, 0, 1);
    vs_fs_color = color;
    gl_Position = projection_matrix  * ( (trans * model_matrix)  * position) ;
}

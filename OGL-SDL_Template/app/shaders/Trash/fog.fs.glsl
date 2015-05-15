#version 430
#define DARKNESS_FACTOR 0.8f
in vec4 vs_fs_color;

layout (location = 0) out vec4 color;

void main(void)
{
    float c = 1-gl_FragCoord.z;
    float r = vs_fs_color.r;
    float g = vs_fs_color.g;
    float b = vs_fs_color.b;
    c = 1-(c*c*c*c);
    for(int i = 0; i < 10; i++){
        c = c*c;
    }
    r = r+c;
    g = g+c;
    b = b+c;
    if( r > 1.0f ){
        r = 1.0f;
    }
    if( g > 1.0f ){
        g = 1.0f;
    }
    if( b > 1.0f ){
        b = 1.0f;
    }
    color = vec4(   r*DARKNESS_FACTOR, 
                    g*DARKNESS_FACTOR, 
                    b*DARKNESS_FACTOR, 
                    1);
	color.a = 0.3;
}

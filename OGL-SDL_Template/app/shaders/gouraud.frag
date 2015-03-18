
#version 400 core

in  vec4 color;

out vec4 fColor;

void
main()
{
    /*fColor = vec4(color.x*color.x*color.x*color.x*color.x*color.x*color.x*color.x*color.x*color.x, 
                    color.y*color.y*color.y*color.y*color.y*color.y*color.y*color.y*color.y*color.y, 
                    color.z*color.z*color.z*color.z*color.z*color.z*color.z*color.z*color.z*color.z, 
                    color.w*color.w*color.w*color.w*color.w*color.w*color.w*color.w*color.w*color.w);//*/
    fColor = color;
}

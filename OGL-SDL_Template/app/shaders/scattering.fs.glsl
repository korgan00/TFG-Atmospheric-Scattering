#version 430

#define N_STEPS 20.0f
#define H_R 7994.0f
#define H_M 1200.0f

in vec4 vs_fs_color;
uniform sampler2D texture_diffuse;
//uniform sampler2D texture2;

layout (location = 0) out vec4 color;

in vec3 obj;
in vec3 cam;

void main(void)
{
    //color = vs_fs_color;
	color = texture(texture_diffuse, vs_fs_color.xy);

	
	vec3 dP = (obj - cam) / N_STEPS;
	vec3 ds = length(dP);

	vec2 density = vec2(0, 0);
	vec3 rayLeigh = vec3(0, 0, 0);
	vec3 mieScattering = vec3(0, 0, 0);

	for (float s = 0.5f; s < N_STEPS; s += 1.0f){

		vec3 point = cam + dP*s;

		float h = point.y; // = lenght(point - Cearth) - Rearth
		vec3 normalEarth = vec3(0, 1, 0); // = point - Cearth / length(point - Cearth)

		vec2 pRM = vec2(exp(-h / H_R), exp(-h / H_R));


	}


}

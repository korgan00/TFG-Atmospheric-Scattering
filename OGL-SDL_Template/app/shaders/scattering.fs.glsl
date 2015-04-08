#version 430

#define N_STEPS 20.0f
#define H_R 7994.0f
#define H_M 1200.0f
#define WORLD_RADIUS 6360000.0f
#define ATM_TOP_HEIGHT 80000.0f
#define ATM_RADIUS WORLD_RADIUS + ATM_TOP_HEIGHT
//#define H_R 79.0f
//#define H_M 10.0f
#define TAM_X 25
#define TAM_Y 25
#define M_PI 3.1415926535897932384626433832795
#define _3_16pi (3 / (16 * M_PI))
#define _3_8pi (3 / (8 * M_PI))
#define G 0.76
#define G2 (G*G)
#define P0 1


uniform sampler2D texture_diffuse;
uniform mat4 projection_matrix;

//uniform sampler2D texture2;
uniform vec3 lightDir;
uniform float lightSun;

uniform vec3 cam;
//uniform float density[TAM_X][TAM_Y];

uniform vec3 betaER;
uniform vec3 betaEM;
uniform vec3 betaSR;
uniform vec3 betaSM;

layout (location = 0) out vec4 color;

in vec4 vs_fs_color;
in vec3 obj;

void main(void)
{
	// Calculo view
	mat4 projTrans = transpose(projection_matrix);
	vec3 view = normalize(projTrans[2].xyz);
	vec3 normLightDir = normalize(lightDir);


	//vec3 dP = (obj - cam) / N_STEPS;
	vec3 delta_P = (obj - cam) / N_STEPS;
	float diferential_s = length(delta_P);

	
	vec2 density_PC = vec2(0, 0);
	vec3 rayLeigh_In = vec3(0, 0, 0);
	vec3 mie_In = vec3(0, 0, 0);

	vec3 cEarth = vec3(0.0f, -WORLD_RADIUS, 0.0f);
	for (float s = 0.5f; s < N_STEPS; s += 1.0f){

		vec3 point = cam + delta_P * s;

		float h = length(point - cEarth) - WORLD_RADIUS;

		if (h < ATM_TOP_HEIGHT) {

			vec3 normalEarth = point - cEarth / length(point - cEarth);

			vec2 partDenRM = P0 * exp(-h / vec2(H_R, H_M));

			//float cosPhi = dot(-normLightDir, normalEarth) / length(normalEarth);
			float cosPhi = dot(normalEarth, -normLightDir);//    / length(normLightDir);

			//vec2 dAP = density[h][cosPhi];
			/******* TO TABLE ********/
			vec2 density_AP = vec2(0.0, 0.0);
			/*vec2 delta_A = (vec2(H_R, H_M) - h) / cosPhi;
			delta_A = delta_A / N_STEPS;
			float diferential_h = length(delta_A);//Cuidado*/
			// CALCULAR PUNTO DE LA ATMOSFERA!!
			float comp_cosPhi = -cosPhi;
			float point_earth = length(point - cEarth);
			float b = 2 * point_earth * cosPhi;
			float c = point_earth * point_earth - ATM_RADIUS * ATM_RADIUS;
			float a1 = (b + sqrt(b*b - 4 * c)) / 2;
			float a2 = (b - sqrt(b*b - 4 * c)) / 2;
			vec3 A1 = -normLightDir * a1 + point;
			vec3 A2 = -normLightDir * a2 + point;
			vec3 A;
			float a1_c = abs(length(A1 - cEarth) - ATM_RADIUS);
			float a2_c = abs(length(A2 - cEarth) - ATM_RADIUS);
			A = a1_c < a2_c ? A1 : A2;

			vec3 delta_A = (point - A) / N_STEPS;
			float diferential_h = length(delta_A);

			for (float step = 0.5f; step < N_STEPS; step += 1.0f) {
				float hPoint = length((A + delta_A * step) - cEarth) - WORLD_RADIUS;
				density_AP += exp(-vec2(hPoint, hPoint) / vec2(H_R, H_M)) * diferential_h;
			}
			//density_AP = vec2(80000, 80000);

			//vec2 dAP = vec2(0.5f, 0.5f);

			density_PC += partDenRM * diferential_s;

			vec2 density_APC = density_AP + density_PC;

			//vec3 tR = density_APC.x * betaER;
			//vec3 tM = density_APC.y * betaEM;

			//vec3 opticalDepthRM = density_APC.x * betaER + density_APC.y * betaEM;
			vec3 extinction_RM = exp(-( density_APC.x * betaER + density_APC.y * betaEM ));

			vec3 difLR = partDenRM.x * betaSR * extinction_RM * diferential_s;
			vec3 difLM = partDenRM.y * betaSM * extinction_RM * diferential_s;

			// Calcular visibilidad de P

			float visi = 1;

			rayLeigh_In += difLR * visi;
			mie_In += difLM * visi;
		}
	}

	//float cosTheta = cos(lightDir + view);
	//float cos2ThetaP1 = 1 + (cosTheta * cosTheta);

	//float cosTheta = dot(normLightDir, -view) / length(view);
	float cosTheta = dot(normalize(-view), normLightDir);//    / length(normLightDir);

	float cos2ThetaP1 = 1 + (cosTheta * cosTheta);

	// Apply Phase Functions 
	// RAY
	float phase_rayLeigh = _3_16pi * cos2ThetaP1;
	// MIE
	float phase_mieScattering = _3_8pi * (((1.0f - G2) * cos2ThetaP1) / ((2.0f + G2) * pow(1.0f + G2 - 2.0f * G*cosTheta, 1.5f)));

	//ApplyPhaseFunctions(rayLeigh, mieScattering, cosTheta);

	//vec3 inScattering = (rayLeigh + mieScattering) * lightSun;
	vec3 inScattering = (rayLeigh_In * phase_rayLeigh + mie_In * phase_mieScattering) * lightSun;
	vec3 extintion = exp(-(density_PC.x * betaER + density_PC.y * betaEM));

	//color = vs_fs_color;
	vec3 L0_Ext = texture(texture_diffuse, vs_fs_color.xy).rgb * extintion;
	color = vec4(1.0f - exp(-1.0f * (L0_Ext + inScattering) ), 1);
	//color = vec4(L0_Ext, 1);
	//color = texture(texture_diffuse, vs_fs_color.xy);
	//color = vec4(L0_Ext + inScattering, 1);
	//color = vec4(phase_mieScattering, phase_mieScattering, phase_mieScattering, 1);
	//color = vec4(inScattering, 1);
	//color = vec4(extintion, 1);
	//*/


}

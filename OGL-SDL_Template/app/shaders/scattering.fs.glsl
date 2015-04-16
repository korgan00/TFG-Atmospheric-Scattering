#version 430

//#define N_STEPS 20.0f

// CONTANTS
uniform float H_R;
uniform float H_M;
uniform float WORLD_RADIUS;
uniform float ATM_TOP_HEIGHT;
uniform float ATM_RADIUS;
uniform float ATM_RADIUS_2;
uniform float M_PI;
uniform float _3_16PI;
uniform float _3_8PI;
uniform float G;
uniform float G2;
uniform float P0;

// PSEUDO-CONTANTS
uniform vec3 lightDir;
uniform float lightSun;

uniform vec3 betaER;
uniform vec3 betaEM;
uniform vec3 betaSR;
uniform vec3 betaSM;

// VARIABLES
uniform sampler2D texture_diffuse;
uniform sampler2D densityRayleigh;
uniform sampler2D densityMie;
uniform mat4 projection_matrix;

uniform vec3 cam;

in vec4 vs_fs_color;
in vec3 obj;

layout (location = 0) out vec4 color;
/*
#define N_STEPS 20.0f
#define H_R 7994.0f
#define H_M 1200.0f
#define WORLD_RADIUS 6360000.0f
#define ATM_TOP_HEIGHT 80000.0f
#define ATM_RADIUS WORLD_RADIUS + ATM_TOP_HEIGHT
//#define H_R 79.0f
//#define H_M 10.0f
#define M_PI 3.1415926535897932384626433832795
#define G 0.76
#define G2 (G*G)
#define P0 1
*/

void main(void)
{
	vec4 transfVec = vec4(256 * 256 * 256, 256 * 256, 256, 1);
	float N_STEPS = 20.0f;

	float _3_16PI = 3 / (16 * M_PI);
	float _3_8PI = 3 / (8 * M_PI);
	//float atm_Radius = ATM_RADIUS;
	//float atm_Radius_2 = ATM_RADIUS * ATM_RADIUS;
	vec3 cEarth = vec3(0.0f, -WORLD_RADIUS, 0.0f);

	// Calculo view
	mat4 projTrans = transpose(projection_matrix);
	vec3 view = normalize(projTrans[2].xyz);
	vec3 normLightDir = normalize(lightDir);

	//float obj_cEarth = length(obj - cEarth);

	vec3 computedCam;

	if (length(obj-cEarth) < ATM_RADIUS) {
		computedCam = normalize(cam - cEarth) * min(ATM_RADIUS, length(cam - cEarth)) + cEarth;
	} else {
		computedCam = cam;
	}

	//vec3 dP = (obj - computedCam) / N_STEPS;
	vec3 delta_P = (obj - computedCam) / N_STEPS;
	float diferential_s = length(delta_P);

	
	vec2 density_PC = vec2(0, 0);
	vec3 rayLeigh_In = vec3(0, 0, 0);
	vec3 mie_In = vec3(0, 0, 0);

	for (float s = 0.5f; s < N_STEPS; s += 1.0f){

		vec3 point = computedCam + delta_P * s;

		float h = length(point - cEarth) - WORLD_RADIUS;

		if (h < ATM_TOP_HEIGHT) {

			//vec3 normalEarth = point - cEarth / length(point - cEarth);
			vec3 normalEarth = normalize(point - cEarth);

			vec2 partDenRM = P0 * exp(-h / vec2(H_R, H_M));

			//float cosPhi = dot(-normLightDir, normalEarth) / length(normalEarth);
			float cosPhi = dot(normalEarth, -normLightDir);//    / length(normLightDir);

			//vec2 dAP = density[h][cosPhi];
			/******* TO TABLE ********/
			//uniform sampler2D densityRayleigh;
			//uniform sampler2D densityMie;
			//float densityHeight = (h / ATM_TOP_HEIGHT);
			//float densityCosPhi = (cosPhi + 1) / 2;
			vec2 densityCoord = vec2((h / ATM_TOP_HEIGHT), (cosPhi + 1) / 2);
			//vec2 densityCoord = vec2((cosPhi + 1) / 2, (h / ATM_TOP_HEIGHT));
			vec4 vDAP_Ray = texture(densityRayleigh, densityCoord);
			vec4 vDAP_Mie = texture(densityMie, densityCoord);
			float fDAP_Ray = dot(transfVec, vDAP_Ray.abgr);
			float fDAP_Mie = dot(transfVec, vDAP_Mie.abgr);
			//float density_AP_Ray = ((density_AP_Ray_v.x * 256 + density_AP_Ray_v.y) * 256 + density_AP_Ray_v.z) * 256 + density_AP_Ray_v.w;
			//float density_AP_Mie = texture(densityMie, densityCoord).rgba;

			vec2 density_AP = vec2(fDAP_Ray, fDAP_Mie);
			/*vec2 delta_A = (vec2(H_R, H_M) - h) / cosPhi;
			delta_A = delta_A / N_STEPS;
			float diferential_h = length(delta_A);//Cuidado*/
			// CALCULAR PUNTO DE LA ATMOSFERA!!
			/*
			float comp_cosPhi = -cosPhi;
			float point_earth = length(point - cEarth);
			float b = 2 * point_earth * comp_cosPhi;
			float c = point_earth * point_earth - ATM_RADIUS_2;
			float sqrtBody = b*b - 4 * c;
			float a1 = (b + sqrt(sqrtBody)) / 2;
			float a2 = (b - sqrt(sqrtBody)) / 2;

			//vec3 A1 = -normLightDir * a1 + point;
			//vec3 A2 = -normLightDir * a2 + point;
			
			//vec3 A1A2 = normalize(A2 - A1);
			//vec3 A2A1 = normalize(A1 - A2);
			//float a1a2 = length(A1A2 - normLightDir);
			//float a2a1 = length(A2A1 - normLightDir);
			//vec3 A = a1a2 < a2a1 ? A1 : A2;
			vec3 A = -normLightDir * max(a1, a2) + point;

			vec3 delta_A = (point - A) / N_STEPS;
			float diferential_h = length(delta_A);

			for (float step = 0.5f; step < N_STEPS; step += 1.0f) {
				float hPoint = (length((A + delta_A * step) - cEarth) - WORLD_RADIUS);
				density_AP += exp( -hPoint / vec2(H_R, H_M)) * diferential_h;
			}/*/
			/*
			vec3 delta_A = (A - point) / N_STEPS;
			float diferential_h = length(delta_A);

			for (float step = 0.5f; step < N_STEPS; step += 1.0f) {
				float hPoint = (length((point + delta_A * step) - cEarth) - WORLD_RADIUS);
				density_AP += exp(-hPoint / vec2(H_R, H_M)) * diferential_h;
			}//*/

			//density_AP = vec2(90000, 12000);
			//density_AP = vec2(10000, 10000);
			//density_AP = vec2(1000, 1000);
			//density_AP = vec2(1000, 1000);

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
	float phase_rayLeigh = _3_16PI * cos2ThetaP1;
	// MIE
	float phase_mieScattering = _3_8PI * (((1.0f - G2) * cos2ThetaP1) / ((2.0f + G2) * pow(1.0f + G2 - 2.0f * G*cosTheta, 1.5f)));

	//ApplyPhaseFunctions(rayLeigh, mieScattering, cosTheta);

	//vec3 inScattering = (rayLeigh + mieScattering) * lightSun;
	vec3 inScattering = (rayLeigh_In * phase_rayLeigh + mie_In * phase_mieScattering) * lightSun;
	vec3 extintion = exp(-(density_PC.x * betaER + density_PC.y * betaEM));

	//color = vs_fs_color;
	vec3 L0_Ext = texture(texture_diffuse, vs_fs_color.xy).rgb * extintion;
	color = vec4(1.0f - exp(-1.0f * (L0_Ext + inScattering) ), 1);
	//color = vec4(L0_Ext, 1);
	//color = texture(texture_diffuse, vs_fs_color.xy);
	//color = vec4(inScattering, 1);
	//color = vec4(phase_mieScattering, phase_mieScattering, phase_mieScattering, 1);
	//color = vec4(inScattering, 1);
	//color = vec4(extintion, 1);
	//*/


}

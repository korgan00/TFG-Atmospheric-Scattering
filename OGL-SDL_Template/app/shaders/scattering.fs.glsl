#version 430

#define N_STEPS 30.0f
#define H_R 7994.0f
#define H_M 1200.0f
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
uniform vec3 cam;
//uniform float density[TAM_X][TAM_Y];
uniform vec3 betaER;
uniform vec3 betaEM;
uniform vec3 betaSR;
uniform vec3 betaSM;
uniform float lightSun;

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
	vec3 dP = (obj - cam) / N_STEPS;
	float ds = length(dP);

	
	vec2 dPC = vec2(0, 0);
	vec3 rayLeigh = vec3(0, 0, 0);
	vec3 mieScattering = vec3(0, 0, 0);

	for (float s = 0.5f; s < N_STEPS; s += 1.0f){

		vec3 point = cam + dP * s;

		float h = point.y; // = lenght(point - Cearth) - Rearth
		vec3 normalEarth = vec3(0, 1, 0); // = point - Cearth / length(point - Cearth)

		vec2 pRM = P0 * exp(-h / vec2(H_R, H_M));

		//float cosPhi = dot(-normLightDir, normalEarth) / length(normalEarth);
		float cosPhi = dot(normalEarth, -normLightDir);//    / length(normLightDir);

		//vec2 dAP = density[h][cosPhi];
		/******* TO TABLE ********/
		vec2 dAP = vec2(0.0, 0.0);
		vec2 dA = (vec2(H_R, H_M) - h) / cosPhi;
		dA = dA / N_STEPS;
		float dh = length(dA);

		for (float step = 0.5f; step < N_STEPS; step += 1.0f) {
			vec2 hPoint = h + dA * step;
			dAP += exp(-hPoint / vec2(H_R, H_M)) * dh;
		}//*/

		//vec2 dAP = vec2(0.5f, 0.5f);

		dPC += pRM * ds;

		vec2 dAPC = dAP + dPC;

		vec3 tR = dAPC.x * betaER;
		vec3 tM = dAPC.y * betaEM;

		vec3 tRM = tR + tM;
		vec3 extRM = exp(-(tRM));

		vec3 difLR = pRM.x * betaSR * extRM * ds;
		vec3 difLM = pRM.y * betaSM * extRM * ds;

		// Calcular visibilidad de P

		float visi = 1;

		rayLeigh += difLR * visi;
		mieScattering += difLM * visi;

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
	vec3 inScattering = (rayLeigh * phase_rayLeigh + mieScattering * phase_mieScattering) * lightSun;
	vec3 extintion = exp(-(dPC.x * betaER + dPC.y * betaEM));

	//color = vs_fs_color;
	vec3 L0_Ext = texture(texture_diffuse, vs_fs_color.xy).rgb *extintion;
	color = vec4(L0_Ext, 1.0f);//vec4(1.0f - exp(-0.9f * (L0_Ext + inScattering) ), 1);
	//color = texture(texture_diffuse, vs_fs_color.xy);
	//color = vec4(L0_Ext + inScattering, 1);
	//color = vec4(phase_mieScattering, phase_mieScattering, phase_mieScattering, 1);
	//color = vec4(inScattering, 1);
	//color = vec4(extintion, 1);
	//*/


}

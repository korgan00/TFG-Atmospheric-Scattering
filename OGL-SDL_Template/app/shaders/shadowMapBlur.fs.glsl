
#version 430 core

uniform sampler2D texture_diffuse;

in vec4 texPos;

layout(location = 0) out vec4 blurfragmentdepth;
//out vec4 outColor;

void main() {
	
	float blurDelta = 3.0f/4096.0f;
	float minClamp = blurDelta * 1.0f;
	float maxClamp = 1.0f - minClamp;
	/*
	float kernel[25] = { 1,  4,  7,  4, 1,
					 	 4, 16, 26, 16, 4,
						 7, 26, 41, 26, 7,
						 4, 16, 26, 16, 4,
						 1,  4,  7,  4, 1};
	*/
						
	float kernel[49] = { 1.0f,   6.0f,  15.0f,  20.0f,  15.0f,   6.0f,  1.0f,
						 6.0f,  36.0f,  90.0f, 120.0f,  90.0f,  36.0f,  6.0f,
						15.0f,  90.0f, 225.0f, 300.0f, 225.0f,  90.0f, 15.0f,
						20.0f, 120.0f, 300.0f, 400.0f, 300.0f, 120.0f, 20.0f,
						15.0f,  90.0f, 225.0f, 300.0f, 225.0f,  90.0f, 15.0f,
						 6.0f,  36.0f,  90.0f, 120.0f,  90.0f,  36.0f,  6.0f,
						 1.0f,   6.0f,  15.0f,  20.0f,  15.0f,   6.0f,  1.0f };

	float count = 4096.0f;

	float depth = 0.0f;
	vec2 firstKPos = texPos.xy - 3.0f * blurDelta;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			vec2 currPos = firstKPos + vec2(i * blurDelta, j * blurDelta);
			depth += texture( texture_diffuse, vec2( clamp(currPos.x, minClamp, maxClamp), 
														clamp(currPos.y, minClamp, maxClamp))).z * kernel[i*7 + j];
		}
	}
	//outColor /= count;
	float newDepth = depth / count;
	blurfragmentdepth = vec4(newDepth, newDepth * newDepth, 0.0f, 1.0f);
	//outColor = texture( texture_diffuse, vec2(clamp(texPos.xy, 0.0f, 1.0f)) );
	//outColor = texture(texture_diffuse, texPos.xy);

}

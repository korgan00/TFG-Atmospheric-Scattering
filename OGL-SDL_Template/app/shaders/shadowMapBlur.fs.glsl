
#version 430 core

uniform sampler2D texture_diffuse;

in vec4 texPos;

layout(location = 0) out vec4 blurfragmentdepth;
//out vec4 outColor;

void main() {
	const int kernelDim = 5;
	const int kernelDim_2 = kernelDim*kernelDim;
	int halfDim = 2;

	float blurDelta = 1.0f/4096.0f;
	float minClamp = blurDelta * 1.0f;
	float maxClamp = 1.0f - minClamp;
	/*
	float kernel[kernelDim_2] = { 1,  4,  7,  4, 1,
					 			  4, 16, 26, 16, 4,
								  7, 26, 41, 26, 7,
								  4, 16, 26, 16, 4,
								  1,  4,  7,  4, 1};
	float count = 273;
	//*/
						
	/*float kernel[kernelDim_2] = {  1.0f,   6.0f,  15.0f,  20.0f,  15.0f,   6.0f,  1.0f,
									 6.0f,  36.0f,  90.0f, 120.0f,  90.0f,  36.0f,  6.0f,
									 15.0f,  90.0f, 225.0f, 300.0f, 225.0f,  90.0f, 15.0f,
									 20.0f, 120.0f, 300.0f, 400.0f, 300.0f, 120.0f, 20.0f,
									 15.0f,  90.0f, 225.0f, 300.0f, 225.0f,  90.0f, 15.0f,
									 6.0f,  36.0f,  90.0f, 120.0f,  90.0f,  36.0f,  6.0f,
									 1.0f,   6.0f,  15.0f,  20.0f,  15.0f,   6.0f,  1.0f };
	float count = 4096.0f;
	//*/

	float kernel[kernelDim_2] = { 1, 1, 1, 1, 1,
					 			  1, 1, 1, 1, 1,
								  1, 1, 1, 1, 1,
								  1, 1, 1, 1, 1,
								  1, 1, 1, 1, 1};
	float count = kernelDim_2;
	//*/
	float depth_2 = 0.0f;
	float depth = 0.0f;
	float edge = 0.0f;
	float currTex = 0.0f;
	vec2 currPos = vec2(0.0f, 0.0f);
	vec2 firstKPos = texPos.xy - halfDim * blurDelta;
	float texDepth = texture(texture_diffuse, texPos.xy).z;
	float bias = 0.0003;

	for (int i = 0; i < kernelDim; i++) {
		for (int j = 0; j < kernelDim; j++) {
			currPos = firstKPos + vec2(i * blurDelta, j * blurDelta);
			currPos.x = clamp(currPos.x, minClamp, maxClamp);
			currPos.y = clamp(currPos.y, minClamp, maxClamp);
			currTex = texture( texture_diffuse, currPos).z;
			
			depth += currTex * kernel[i*kernelDim + j];
			depth_2 += currTex * currTex * kernel[i*kernelDim + j];
			edge += currTex < texDepth? 1.0f : 0.0f;
		}
	}
	//outColor /= count;
	float newDepth = depth / count;
	float newDepth_2 = depth_2 / count;
	float oldDepth = texture( texture_diffuse, texPos.xy ).z;
	float theEdge = edge / count;

	blurfragmentdepth = vec4(newDepth, newDepth_2, oldDepth, theEdge);
	//outColor = texture(texture_diffuse, texPos.xy);

}

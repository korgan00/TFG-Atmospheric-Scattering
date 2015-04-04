#ifndef MOUNTAIN_TEXTURE_FACTORY_H_
#define MOUNTAIN_TEXTURE_FACTORY_H_

#include "ObjToMesh.h"

class MountainTextureFactory : public ObjNameToTextureName {
public:
	string getDiffuseTexName(string objName) {
		stringstream ss;
		ss << "../OGL-SDL_Template/app/resources/ObjTex/QuantumArid_Diffuse_" << objName[10] << ".jpg";
		return ss.str();
	}
	string getNormalMapTexName(string objName) {
		stringstream ss;
		ss << "../OGL-SDL_Template/app/resources/ObjTex/QuantumArid_Diffuse_" << objName[10] << ".jpg";
		return ss.str();
	}

};

#endif /* MOUNTAIN_TEXTURE_FACTORY_H_ */
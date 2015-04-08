
#ifndef SCATT_SCENE_H_
#define SCATT_SCENE_H_
#include "Scene.h"
#include "mesh/ObjLoader.h"
#include "mesh/ObjToMesh.h"
#include "mesh/MountainTextureFactory.h"

class ScatteringScene : public Scene {
private:

	static const GLfloat   EARTH_RADIUS;
	static const GLfloat   ATM_TOP_HEIGHT;
	static const GLfloat   DEEP_SPACE_RADIUS;
	static const GLfloat   MOUNTAINS_SCALE;

	static const GLfloat   PARTICLE_SCALE_HEIGHT_RAYLEIGH;
	static const GLfloat   PARTICLE_SCALE_HEIGHT_MIE;

	Mesh* _mountains;
	Mesh* _blueSphere;
	Mesh* _deepSpace;
public:
	void initOGLData();
};

#endif /* SCATT_SCENE_H_ */
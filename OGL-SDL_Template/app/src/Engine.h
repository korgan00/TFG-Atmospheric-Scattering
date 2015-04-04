/*
* Engine.h
*
*  Created on: 15/03/2015
*      Author: Korgan
*/

#ifndef ENGINE_H_
#define ENGINE_H_
#include <iostream>
#include <string>
#include <sstream>

#include "../common/common.h"
#include "mesh/ObjLoader.h"
#include "CameraFPS.h"
//#include "Scene.h"
#include "mesh/ObjLoader.h"
#include "mesh/ObjToMesh.h"
#include "mesh/MountainTextureFactory.h"

class Engine {
private:
	bool _running;
	SDL_Window* _window;
	SDL_GLContext _ctxt;

	static const uint32_t   WIN_HEIGHT = DEFAULT_WIN_HEIGHT;
	static const uint32_t   WIN_WIDTH = DEFAULT_WIN_WIDTH;
	static const char*      WIN_TITLE; //px
	Info_Manager _info;

	GLfloat _old_t;
	GLfloat _t;
	GLfloat _dt;

	/***************************************************/

	CameraFPS _camera;
	//Scene _scene;
	//vector<Mesh> _scene;
	//vector<Mesh> _cube;
	Mesh *_mesh;

public:

	Engine();
	/*
	* GAME LOOP FUNCTIONS
	*/

	int Execute(){ return OnExecute(); }
	bool Init(){ return OnInit(); }
	void Loop(){ return OnLoop(); }
	void Render(){ return OnRender(); }
	void Cleanup(){ return OnCleanup(); }
	void Event(SDL_Event* Event){ OnEvent(Event); }


	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* Event);
	void OnLoop();
	void OnRender();
	void OnCleanup();

	/***************************************************/

	void SetupOpenGL();
	void InitData();
	void Engine::InitMountainTextures(vector<Mesh> mountains);

};


#endif /* ENGINE_H_ */

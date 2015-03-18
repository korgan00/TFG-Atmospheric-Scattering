/*
* Demo_Mesh.h
*
*  Created on: 10/03/2015
*      Author: Korgan
*/

#ifndef DEMO_OGL_MESH_H_
#define DEMO_OGL_MESH_H_
#include <iostream>
#include <string>
#include <sstream>

#include "../common/common.h"
#include "ObjLoader.h"
#include "CameraFPS.h"



class Demo_OGL_Mesh {
private:
	bool running;
	SDL_Window* window;
	SDL_GLContext ctxt;

	static const uint32_t   WIN_HEIGHT = DEFAULT_WIN_HEIGHT;
	static const uint32_t   WIN_WIDTH = DEFAULT_WIN_WIDTH;
	static const char*      WIN_TITLE; //px
	Info_Manager info;
	/***************************************************/
	/***************************************************/
	/***************************************************/


	float aspect;
	static const GLfloat cube_positions[];
	static const GLfloat cube_colors[];
	static const GLushort cube_indices[];
	static const GLfloat cube_position1[];
	static const GLfloat cube_position2[];
	static const vmath::vec3 X;
	static const vmath::vec3 Y;
	static const vmath::vec3 Z;

	vector<Mesh> scene;
	CameraFPS camera;
	GLfloat lastFrameTime;

public:

	Demo_OGL_Mesh();

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
	/***************************************************/
	/***************************************************/

	void SetupOpenGL();
	void InitData();
};


#endif /* DEMO_OGL_MESH_H_ */

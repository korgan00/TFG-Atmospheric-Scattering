/*
* Engine.h
*
*  Created on: 01/07/2013
*      Author: Korgan
*/

#ifndef ENGINE_H_
#define ENGINE_H_
#include <iostream>

#include "../common/common.h"

class Engine {
private:
	bool running;
	SDL_Window* window;
	SDL_GLContext ctxt;

	static const uint32_t   WIN_HEIGHT = DEFAULT_WIN_HEIGHT;
	static const uint32_t   WIN_WIDTH = DEFAULT_WIN_WIDTH;
	static const char*      WIN_TITLE; //px
	Info_Manager info;

	/***************************************************/

	enum Attrib_Locs { vPosition = 0, vColor = 1 };

	GLuint  vao[1];
	GLuint  buffer[1];

	static const GLuint NumVertices = 3;
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
};


#endif /* ENGINE_H_ */

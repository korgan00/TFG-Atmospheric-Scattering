
#include "../common/common.h"
#include <iostream>
#include <vector>
#include "mesh/ObjLoader.h"
#include <time.h>

//#include "Demo_OGL_4.h"
//#include "Demo_OGL_Mesh.h"
//#include "Engine.h"



int main(int argc, char* argv[]) {
	//std::cin.get();
	//Engine* d = new Engine();
	//d->Execute();
	//vector<Mesh> vec = ObjLoader::load("Arid.obj");
	//DoTheImportThing("../OGL-SDL_Template/app/resources/Cubo.obj");
	/*
	Log::error("test");
	Log::warning("test");
	Log::info("test");
	Log::debug("test");
	Log::trace("test");
	cout << "test" << endl;
	*/
	cout << endl << endl;

	//ObjLoader loader;
	clock_t start2 = clock(), diff2;
	ObjLoader::ObjFileInfo* obj = ObjLoader::load("Earth.obj");

	diff2 = clock() - start2;

	int msec = diff2 * 1000 / CLOCKS_PER_SEC;
	stringstream ss;
	ss << "Time taken " << (msec / 1000) << " seconds " << (msec % 1000) << " milliseconds";
	Log::info(ss.str());

	std::cin.get();
	return 0;
}

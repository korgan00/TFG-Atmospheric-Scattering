
#include <iostream>
#include <iomanip>
#include "Demo_OGL_4.h"
#include "Demo_OGL_Mesh.h"
#include "ObjLoader.h"
#include <vector>

int main(int argc, char* argv[]) {
	Demo_OGL_Mesh* d = new Demo_OGL_Mesh();
	d->Execute();
	//vector<Mesh> vec = ObjLoader::load("Arid.obj");
	std::cin.get();
	return 0;
}
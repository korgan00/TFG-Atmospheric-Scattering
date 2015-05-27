
#include "../common/common.h"
#include <iostream>
#include <vector>
#include <time.h>

//#include "Demo_OGL_4.h"
//#include "Demo_OGL_Mesh.h"
#include "Engine.h"



void optimizeOBJ();

int main(int argc, char* argv[]) {
	
	Engine* d = new Engine();
	d->Execute(); 
	/*/

	optimizeOBJ();
	cout << "YA";//*/
	std::cin.get();
	

	return 0;
}


void optimizeOBJ() {
	ifstream input("../OGL-SDL_Template/app/resources/Arid.obj");

	char buffer[256];
	vector<string> vs;
	string mtlLib = "";
	typedef vector<string> strVec;
	strVec others[10];
	int lastMtl = 0;


	while (!input.eof()) {
		input.getline(buffer, 256);
		if (buffer[0] == 'v') {
			vs.push_back(string(buffer));
		} else if (buffer[0] == 'm') {
			mtlLib = string(buffer);
		} else if (buffer[0] == '#') {
		} else if (buffer[0] == 'g') {
		} else if (buffer[0] == 'u') {
			lastMtl = buffer[18] - (int)'A';
			if (others[lastMtl].size() == 0) {
				others[lastMtl].push_back(string(buffer));
			}
		} else if (buffer[0] == 'f') {
			others[lastMtl].push_back(string(buffer));
		}
	}
	
	ofstream output("../OGL-SDL_Template/app/resources/AridOpti.obj");

	output << endl;
	output << endl;
	output << mtlLib << endl;
	output << endl;
	output << endl;

	for (vector<string>::iterator str = vs.begin(); str != vs.end(); ++str) {
		output << *str << endl;
	}

	for (int i = 0; i < 10; i++) {
		output << endl;
		output << endl;
		output << "g" << endl;
		for (vector<string>::iterator str = others[i].begin(); str != others[i].end(); ++str) {
			output << *str << endl;
		}
	}
	
	input.close();
	output.close();
	
}



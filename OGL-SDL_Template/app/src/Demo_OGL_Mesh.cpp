/*
* Demo_Mesh.cpp
*
*  Created on: 10/03/2015
*      Author: Korgan
*
*  mingw32
*  glew32
*  opengl32
*  SDL2main
*  SDL2
*
*/

#include "Demo_OGL_Mesh.h"

const char* Demo_OGL_Mesh::WIN_TITLE = "Titulo de la Ventana";
/*const GLfloat Demo_OGL_Mesh::cube_positions[] = {
	-1.0f, -1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};
const GLfloat Demo_OGL_Mesh::cube_colors[] = {
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f
};
const GLfloat Demo_OGL_Mesh::cube_position1[] = { -2.0f, 0.0f, 5.0f };
const GLfloat Demo_OGL_Mesh::cube_position2[] = { 2.0f, 0.0f, 5.0f };

const GLushort Demo_OGL_Mesh::cube_indices[] = {
	0, 1, 2, 3, 6, 7, 4, 5, // First strip
	0xFFFF, // <<-- This is the restart index
	2, 6, 0, 4, 1, 5, 3, 7 // Second strip
};

const vmath::vec3 Demo_OGL_Mesh::X(1.0f, 0.0f, 0.0f);
const vmath::vec3 Demo_OGL_Mesh::Y(0.0f, 1.0f, 0.0f);
const vmath::vec3 Demo_OGL_Mesh::Z(0.0f, 0.0f, 1.0f);
*/
Demo_OGL_Mesh::Demo_OGL_Mesh() : running(false), window(NULL), ctxt(NULL), info(), aspect(0), camera(NULL) {}

void Demo_OGL_Mesh::OnEvent(SDL_Event* event) {
	camera.Event(event);
	switch (event->type) {
	case SDL_KEYUP:
		switch (event->key.keysym.sym){
		case SDLK_v:
			std::cout << std::endl;
			std::cout << info.client_info() << std::endl;
			break;
		case SDLK_o:
			camera.mouseIsCaptured(!camera.mouseIsCaptured());
			break;
		case SDLK_p:
			std::cout << " - Current position: " << camera.position()[0] << ", " << camera.position()[1] << ", " << camera.position()[2] << std::endl;
			break;
		case SDLK_ESCAPE:
			running = false;
			break;
		default:
			break;
		}
		break;
	case SDL_QUIT:
		running = false;
		break;
	default:
		break;
	}
}
void Demo_OGL_Mesh::OnLoop() {
	GLfloat now = (GLfloat)GetTickCount();
	GLfloat elapsedTime = now - lastFrameTime;
	camera.tick(now, elapsedTime);
	lastFrameTime = now;
}

void Demo_OGL_Mesh::OnCleanup() {
	glUseProgram(0);

	forEach(scene, &Mesh::cleanup);

	SDL_GL_DeleteContext(ctxt);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Demo_OGL_Mesh::OnInit() {
	cout << "Initializing SDL" << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	cout << "Initializing IMG" << endl;
	if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG)
		return false;

	cout << "Creating a window" << endl;
	window = SDL_CreateWindow(WIN_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH, WIN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window)
		return false;

	cout << "Setting up OGL" << endl;
	SetupOpenGL();
	cout << "Initializing OpenGL data" << endl;
	InitData();

	running = true;

	return true;
}
void Demo_OGL_Mesh::SetupOpenGL(){

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	ctxt = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(0);

	if (gl3wInit()) {
		std::cout << "Error al Inicializar GL3W" << std::endl;
	}
}
int Demo_OGL_Mesh::OnExecute() {
	
	if (!Init())
		return -1;

	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event))
			Event(&event);

		Loop();
		Render();

		info.frame(window, WIN_TITLE);
	}
	Cleanup();
	return 0;
}

void Demo_OGL_Mesh::InitData(){

	camera = CameraFPS(window);

	aspect = float(WIN_HEIGHT) / float(WIN_WIDTH);

	scene = ObjLoader::loadMountains("Arid.obj");


	forEach(scene, &Mesh::initOGLData);

	//Seleccionamos el color de fondo
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	//Los triangulos que no se ven, ni los pinta (los triangulos que estan de espaldas)
	glEnable(GL_CULL_FACE);
	//Utiliza el z buffer
	glEnable(GL_DEPTH_TEST);
	//Funcion para el z-buffer
	glDepthFunc(GL_LESS);
	//glFrontFace(GL_CW);
	//glCullFace(GL_FRONT_AND_BACK);

	//lo que ocupa la parte en la que se dibuja.
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}

void Demo_OGL_Mesh::OnRender() {

	float t = float(GetTickCount() & 0x1FFF) / float(0x1FFF);

	// Calculamos la matriz modelo
	//vmath::mat4 model_matrix(vmath::translate(0.0f, 0.0f, 0.0f)
	/*
									vmath::rotate(t * 360.0f, Y) *
									vmath::rotate(t * 720.0f, Z)*/
	//);
	// Calculamos la matriz de proyección mediante un frustum
	//vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 2000.0f) * camera.matrix());

	// Limpiamos el buffer de profundidad (se pone al valor por defecto)
	// y el de color (se pone al valor de glClearColor
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (std::vector<Mesh>::iterator mesh = scene.begin(); mesh != scene.end(); ++mesh) {
		mesh->draw(camera.matrix());
	}
	//SDL_SetWindowGrab(window, !SDL_GetWindowGrab(window));
	// Buffer swap
	SDL_GL_SwapWindow(window);
}

void Demo_OGL_Mesh::forEach(std::vector<Mesh> &bunch, void(Mesh::*f)()) {
	for (std::vector<Mesh>::iterator mesh = bunch.begin(); mesh != bunch.end(); ++mesh) {
		(*mesh.*f)();
	}
}











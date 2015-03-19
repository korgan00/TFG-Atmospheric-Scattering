
#include "Engine.h"

const char* Engine::WIN_TITLE = "Titulo de la Ventana";

Engine::Engine() : _running(false), _window(NULL), _ctxt(NULL), _info(), 
					_old_t((GLfloat)GetTickCount()), _t(0.0f), _dt(0.0f), 
					_camera(NULL), _scene() {}

int Engine::OnExecute() {
	if (!Init()) return -1;

	SDL_Event event;

	while (_running) {
		while (SDL_PollEvent(&event))
			Event(&event);

		Loop();
		Render();

		_info.frame(_window, WIN_TITLE);
	}

	Cleanup();
	return 0;
}

bool Engine::OnInit() {
	cout << "Initializing SDL" << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;

	cout << "Initializing IMG Library" << endl;
	if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG) return false;

	cout << "Creating a window" << endl;
	_window = SDL_CreateWindow(WIN_TITLE,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIN_WIDTH, WIN_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (!_window) return false;

	cout << "Setting up OGL" << endl;
	SetupOpenGL();
	cout << "Initializing OpenGL data" << endl;
	InitData();

	_running = true;

	return true;
}
void Engine::SetupOpenGL() {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	_ctxt = SDL_GL_CreateContext(_window);

	SDL_GL_SetSwapInterval(1);

	if (gl3wInit()) {
		std::cout << "Error al Inicializar GL3W" << std::endl;
	}

	if (!gl3wIsSupported(4, 3)){
		std::cout << "OpenGL 4.3 no es soportado" << std::endl;
	}
}

void Engine::InitData() {
	_camera = CameraFPS(_window);

	_scene = ObjLoader::loadMountains("Arid.obj");

	forEach(_scene, &Mesh::initOGLData);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}



void Engine::OnEvent(SDL_Event* event) {
	_camera.Event(event);
	switch (event->type) {
		case SDL_KEYUP:
			switch (event->key.keysym.sym){
			case SDLK_v:
				std::cout << glGetString(GL_VERSION) << std::endl;
				break;
			case SDLK_o:
				_camera.mouseIsCaptured(!_camera.mouseIsCaptured());
				break;
			case SDLK_p:
				std::cout << " - Current position: " << _camera.position()[0] << ", " << _camera.position()[1] << ", " << _camera.position()[2] << std::endl;
				break;
			case SDLK_ESCAPE:
				_running = false;
				break;
			default:
				break;
			}
			break;
		case SDL_QUIT:
			_running = false;
			break;
		default:
			break;
	}
}
void Engine::OnLoop() {
	_t = (GLfloat)GetTickCount();
	_dt = _t - _old_t;
	_old_t = _t;

	_camera.tick(_t, _dt);

}

void Engine::OnCleanup() {
	glUseProgram(0); //clear shader

	forEach(_scene, &Mesh::cleanup);

	SDL_GL_DeleteContext(_ctxt);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}



void Engine::OnRender() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (std::vector<Mesh>::iterator mesh = _scene.begin(); mesh != _scene.end(); ++mesh) {
		mesh->draw(_camera.matrix());
	}

	SDL_GL_SwapWindow(_window);
}














#include "CameraFPS.h"

const GLfloat CameraFPS::MOVMENT = 0.05f;
const GLfloat CameraFPS::ROTATION = 0.15f;

CameraFPS::CameraFPS() : _velocity(0, 0, 0), _rotation(0, 0, 0) {
	_currentMatrix = vmath::lookat( vmath::vec3(0, 0,  0), 
									vmath::vec3(0, 0, -1), 
									vmath::vec3(0, 1,  0));
}

void CameraFPS::Event(SDL_Event* event) {
	switch (event->type) {
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym){
		case SDLK_w:	 move(Z, NEGATIVE); break;
		case SDLK_s:	 move(Z, POSITIVE); break;
		case SDLK_q:	 move(X, NEGATIVE); break;
		case SDLK_e:	 move(X, POSITIVE); break;
		case SDLK_LCTRL: move(Y, NEGATIVE); break;
		case SDLK_SPACE: move(Y, POSITIVE); break;

		case SDLK_a:	 rotate(Y, NEGATIVE); break;
		case SDLK_d:	 rotate(Y, POSITIVE); break;
		case SDLK_r:	 rotate(X, NEGATIVE); break;
		case SDLK_f:	 rotate(X, POSITIVE); break;
		default:;
		}
		break;
	case SDL_KEYUP:
		switch (event->key.keysym.sym){
		case SDLK_w:	 move(Z, ZERO); break;
		case SDLK_s:	 move(Z, ZERO); break;
		case SDLK_q:	 move(X, ZERO); break;
		case SDLK_e:	 move(X, ZERO); break;
		case SDLK_SPACE: move(Y, ZERO); break;
		case SDLK_LCTRL: move(Y, ZERO); break;

		case SDLK_a:	 rotate(Y, ZERO); break;
		case SDLK_d:	 rotate(Y, ZERO); break;
		case SDLK_r:	 rotate(X, ZERO); break;
		case SDLK_f:	 rotate(X, ZERO); break;
		default:;
		}
		break;
	default:
		break;
	}
}
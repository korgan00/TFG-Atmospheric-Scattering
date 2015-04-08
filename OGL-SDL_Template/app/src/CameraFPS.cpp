
#include "CameraFPS.h"

const GLfloat CameraFPS::MOVMENT = 0.05f;
const GLfloat CameraFPS::ROTATION = 0.15f;
const GLfloat CameraFPS::MOUSE_ROTATION = 180.0f;
const GLfloat CameraFPS::RENDER_DISTANCE = 14000000.0f;

CameraFPS::CameraFPS(SDL_Window* w) :
		_captureMouse(false), _wrapping(false), _window(w), _velocity(0, 0, 0),
		_rotating(0, 0, 0), _xRotation(0), _yRotation(0), _updateRotation(true),
		_currentTranslation(vmath::mat4::identity()), _disableCamera(false), _turbo(GL_FALSE) {

	int winWidth, winHeight;
	SDL_GetWindowSize(_window, &winWidth, &winHeight);
	GLfloat aspect = float(DEFAULT_WIN_HEIGHT) / float(DEFAULT_WIN_WIDTH);
	_currentMatrix = vmath::mat4(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 2000.0f));
}

void CameraFPS::Event(SDL_Event* event) {
	if (_disableCamera) {
		if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_t) {
				_disableCamera = false;
		}
		return;
	}

	switch (event->type) {
	case SDL_MOUSEMOTION: if (_captureMouse) { mouseMotionCaptured(event); } break;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym){
		case SDLK_t:	 disabledCamera(true); break;
		case SDLK_s:	 move(Z, NEGATIVE); break;
		case SDLK_w:	 move(Z, POSITIVE); break;
		case SDLK_a:	 move(X, NEGATIVE); break;
		case SDLK_d:	 move(X, POSITIVE); break;
		case SDLK_LCTRL: move(Y, NEGATIVE); break;
		case SDLK_SPACE: move(Y, POSITIVE); break;
		case SDLK_z:	 turbo(!turbo());	break;

		case SDLK_q:	 rotate(Y, NEGATIVE); break;
		case SDLK_e:	 rotate(Y, POSITIVE); break;
		case SDLK_r:	 rotate(X, NEGATIVE); break;
		case SDLK_f:	 rotate(X, POSITIVE); break;
		default:;
		}
		break;
	case SDL_KEYUP:
		switch (event->key.keysym.sym){
		case SDLK_s:	 move(Z, ZERO); break;
		case SDLK_w:	 move(Z, ZERO); break;
		case SDLK_a:	 move(X, ZERO); break;
		case SDLK_d:	 move(X, ZERO); break;
		case SDLK_SPACE: move(Y, ZERO); break;
		case SDLK_LCTRL: move(Y, ZERO); break;

		case SDLK_q:	 rotate(Y, ZERO); break;
		case SDLK_e:	 rotate(Y, ZERO); break;
		case SDLK_r:	 rotate(X, ZERO); break;
		case SDLK_f:	 rotate(X, ZERO); break;
		default:;
		}
		break;
	default:
		break;
	}
}


void CameraFPS::mouseIsCaptured(GLboolean state) {
	_captureMouse = state;
	_wrapping = state;
	SDL_bool sdl_captured = _captureMouse ? SDL_TRUE : SDL_FALSE;
	SDL_ShowCursor(!sdl_captured);
	SDL_SetWindowGrab(_window, sdl_captured);

	if (_captureMouse) {
		int winWidth, winHeight;
		SDL_GetWindowSize(_window, &winWidth, &winHeight);
		SDL_WarpMouseInWindow(_window, winWidth / 2, winHeight / 2);
	}
}

void CameraFPS::tick(GLfloat time, GLfloat elapsedTime) {
	GLboolean updateTranslation = _velocity[0] != 0 || _velocity[1] != 0 || _velocity[2] != 0;
	_updateRotation = _updateRotation || _rotating[0] != 0 || _rotating[1] != 0;
	
	if (_updateRotation || updateTranslation) {
		rotate(X, _rotating[0] * ROTATION * elapsedTime);
		rotate(Y, _rotating[1] * ROTATION * elapsedTime);

		_updateRotation = false;
		vmath::mat4 currentRotation = vmath::rotate(_xRotation, vmath::vec3(1, 0, 0)) * vmath::rotate(_yRotation, vmath::vec3(0, 1, 0));

		if (updateTranslation) {
			vmath::mat4 movmentRotation = vmath::rotate(_yRotation, vmath::vec3(0, 1, 0)) * vmath::rotate(_xRotation, vmath::vec3(1, 0, 0));
			vmath::vec4 translation = -(movmentRotation * vmath::translate(_velocity * elapsedTime))[3];

			translation[3] = 0;
			translation[2] = -translation[2];
			if (_turbo) translation *= 200;

			_currentTranslation[3] = _currentTranslation[3] + translation;
		}
		int winWidth, winHeight;
		SDL_GetWindowSize(_window, &winWidth, &winHeight);
		GLfloat aspect = float(DEFAULT_WIN_HEIGHT) / float(DEFAULT_WIN_WIDTH);

		_currentMatrix = vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, RENDER_DISTANCE) * (currentRotation * _currentTranslation);
	}

}
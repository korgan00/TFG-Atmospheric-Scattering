/*
* CameraFPS.h
*
*  Created on: 16/03/2015
*      Author: Korgan
*/

#ifndef CAMERA_FPS_H_
#define CAMERA_FPS_H_
#include <iostream>
#include <math.h>

#include "../common/common.h"

class CameraFPS {
private:
	static const GLfloat MOVMENT;
	static const GLfloat ROTATION;
	static const GLfloat MOUSE_ROTATION;
	static const GLfloat RENDER_DISTANCE;

	vmath::vec3 _velocity;
	vmath::vec3 _rotating;

	vmath::mat4 _currentMatrix;
	vmath::mat4 _currentTranslation;
	GLfloat _xRotation;
	GLfloat _yRotation;

	SDL_Window* _window;
	GLboolean _captureMouse;
	GLboolean _disableCamera;
	GLboolean _wrapping;
	GLboolean _updateRotation;

	void mouseMotionCaptured(SDL_Event* event) {
		if (_wrapping) {
			_wrapping = false;
			return;
		}

		int winWidth, winHeight;
		float relX, relY;
		SDL_GetWindowSize(_window, &winWidth, &winHeight);
		relX = event->motion.xrel / (float)winWidth;
		relY = event->motion.yrel / (float)winHeight;

		if (relX != 0.0f) { rotate(Y, relX * MOUSE_ROTATION); }
		if (relY != 0.0f) { rotate(X, relY * MOUSE_ROTATION); }
		_wrapping = true;
		SDL_WarpMouseInWindow(_window, winWidth / 2, winHeight / 2);
	}

public:

	static const enum Axis { X = 0, Y = 1, Z = 2 };
	static const enum Way { POSITIVE = 1, NEGATIVE = -1, ZERO = 0 };

	CameraFPS(SDL_Window* w);

	void Event(SDL_Event*);

	void tick(GLfloat time, GLfloat elapsedTime);

	inline void move(vmath::vec3 v) { _currentMatrix = vmath::translate(v) * _currentMatrix; }
	inline void move(Axis axis, Way way) { _velocity[axis] = MOVMENT * way; }
	
	inline void rotate(Axis axis, GLfloat angle) {
		if (axis == Y) _yRotation +=angle;
		else if (axis == X) _xRotation = fmax(fmin(_xRotation + angle, 90.0f), -90.0f);
		_updateRotation = true;
	}
	inline void rotate(Axis axis, Way way) { _rotating[axis] = (float)way; }

	inline vmath::mat4 matrix() { return _currentMatrix; }
	inline void matrix(vmath::mat4 m) { _currentMatrix = m; }

	inline vmath::vec4 position() { return -_currentTranslation[3]; }
	inline void position(vmath::vec4 v) { _currentTranslation[3] = -v; }

	void mouseIsCaptured(GLboolean state);
	inline GLboolean mouseIsCaptured() { return _captureMouse; }

	void disabledCamera(GLboolean state) { _disableCamera = state; }
	inline GLboolean disabledCamera() { return _disableCamera; }
};


#endif /* CAMERA_FPS_H_ */

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

	vmath::vec3 _velocity;
	vmath::vec3 _rotation;

	vmath::mat4 _currentMatrix;
public:

	static const enum Axis { X = 0, Y = 1, Z = 2 };
	static const enum Way { POSITIVE = 1, NEGATIVE = -1, ZERO = 0 };

	CameraFPS();

	void Event(SDL_Event*);

	void tick(GLfloat time, GLfloat elapsedTime) {
		GLfloat deltaAngle = length(_rotation) * ROTATION * elapsedTime;
		
		if (abs(deltaAngle) > FLT_EPSILON) {
			_currentMatrix = vmath::rotate(deltaAngle, _rotation) * _currentMatrix;
		}

		if (_velocity[0] != 0 || _velocity[1] != 0 || _velocity[2] != 0) {
			_currentMatrix = vmath::translate(_velocity * -elapsedTime) * _currentMatrix;
		}
	}

	void move(vmath::vec3 v) {
		_currentMatrix = vmath::translate(v) * _currentMatrix;
	}
	void move(Axis axis, Way way) {
		_velocity[axis] = MOVMENT * way;
	}
	
	void rotate(Axis axis, GLfloat angle) {
		vmath::vec3 v(0, 0, 0); v[axis] = 1;
		_currentMatrix = vmath::rotate(angle, v) * _currentMatrix;
	}
	void rotate(Axis axis, Way way) {
		_rotation[axis] = (float)way;
	}

	vmath::mat4 matrix() {
		return _currentMatrix;
	}

	void matrix(vmath::mat4 m) {
		_currentMatrix = m;
	}

	vmath::vec4 position() {
		return -_currentMatrix[3];
	}
	void position(vmath::vec4 v) {
		_currentMatrix[3] = -v;
	}
};


#endif /* CAMERA_FPS_H_ */

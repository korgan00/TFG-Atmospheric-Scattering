
#include "common.h"

void CheckErr() {
	GLenum err = glGetError();
	stringstream ss;

	while (err != GL_NO_ERROR) {
		string error;

		switch (err) {
			case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
			case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
			case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
			case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		ss.clear();
		ss << "GL_" << error.c_str() << endl;
		Log::error(ss.str());
		err = glGetError();
	}
}

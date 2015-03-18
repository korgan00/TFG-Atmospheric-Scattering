/*
 * FPS_Ctrlr.h
 *
 *  Created on: 17/07/2013
 *      Author: Korgan
 */

#ifndef FPS_CTRLR_H_
#define FPS_CTRLR_H_
#include <iostream>
#include <string>
#include <sstream>
#include <GL/gl3w.h>
#include "SDL.h"


class Info_Manager {
private:
    Uint32 _last_time_FPS;
    Uint32 _frames;
    Uint32 _fps;
public:
    Info_Manager() : _last_time_FPS(SDL_GetTicks()), _frames(0), _fps(0){}

	void frame(SDL_Window* window, const char* win_title){
        _frames++;
		Uint32 t = SDL_GetTicks();
        //Por si llega al limite de los int
		if (_last_time_FPS - 250 > t){
            _last_time_FPS = t;
            _frames = 0;
        }
		if ((_last_time_FPS + 250) <= t){
            _last_time_FPS += 250;
            _fps = _frames * 4;
            _frames = 0;
			fps_ogl_ver_on_window(window, win_title);
        }
    }
    Uint32 fps(){
        return _fps;
    }
    void fps_ogl_ver_on_window(SDL_Window* window, const char* win_title){
        std::stringstream sstring;
        sstring << win_title << " [FPS: " << _fps
                << ", OGL-version: " << glGetString(GL_VERSION)
                << "]";
        std::string s = sstring.str();
        SDL_SetWindowTitle(window, s.c_str());
    }

    std::string client_info(){
        std::stringstream sstring;
        sstring << "--------------------------------------------------------" << std::endl
                << " + OGL-version: " << glGetString(GL_VERSION) << std::endl
                << " + GLSL-version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl
                << " + Vendor: " << glGetString(GL_VENDOR) << std::endl
                << " + Renderer: " << glGetString(GL_RENDERER) << std::endl
                << "--------------------------------------------------------" << std::endl;
        std::string s = sstring.str();
        return s;
    }

};

#endif /* FPS_CTRLR_H_ */

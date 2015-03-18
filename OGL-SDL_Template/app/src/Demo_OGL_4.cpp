/*
 * Demo_OGL_4.cpp
 *
 *  Created on: 01/07/2013
 *      Author: Korgan
 *
 *
 */

#include "Demo_OGL_4.h"
#include <climits>

const char* Demo_OGL_4::WIN_TITLE = "Titulo de la Ventana";
const GLfloat Demo_OGL_4::VELOCITY = 0.01f;

const GLfloat Demo_OGL_4::cube_positions[] = {
        -0.3f, -0.3f, -0.3f, 1.0f,
        -0.3f, -0.3f,  0.3f, 1.0f,
        -0.3f,  0.3f, -0.3f, 1.0f,
        -0.3f,  0.3f,  0.3f, 1.0f,
         0.3f, -0.3f, -0.3f, 1.0f,
         0.3f, -0.3f,  0.3f, 1.0f,
         0.3f,  0.3f, -0.3f, 1.0f,
         0.3f,  0.3f,  0.3f, 1.0f
};
const GLfloat Demo_OGL_4::cube_colors[] = {
         1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  0.0f,  1.0f,
         1.0f,  0.0f,  1.0f,  1.0f,
         1.0f,  0.0f,  0.0f,  1.0f,
         0.0f,  1.0f,  1.0f,  1.0f,
         0.0f,  1.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f
};
const GLushort Demo_OGL_4::cube_indices[] = {
        0, 1, 2, 3, 6, 7, 4, 5, // First strip
        0xFFFF, // <<-- This is the restart index
        2, 6, 0, 4, 1, 5, 3, 7 // Second strip*/
};

const vmath::vec3 Demo_OGL_4::X(1.0f, 0.0f, 0.0f);
const vmath::vec3 Demo_OGL_4::Y(0.0f, 1.0f, 0.0f);
const vmath::vec3 Demo_OGL_4::Z(0.0f, 0.0f, 1.0f);

Demo_OGL_4::Demo_OGL_4() : running(false), window(NULL), ctxt(NULL), info(),
			aspect(0), render_prog(0), camera(), 
            render_model_matrix_loc(0), render_projection_matrix_loc(0),
			lastFrameTime((GLfloat)GetTickCount()),
			projection_matrix(vmath::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f)) {
	
	ebo[0] = vao[0] = vbo[0] = -1;
	vbo[1] = -1;

    for(int32_t i = 0; i < INST_LENGTH; i++){
        for(int32_t j = 0; j < INST_LENGTH; j++){
            for(int32_t k = 0; k < INST_LENGTH; k++){
				
                cube_rel_pos[i*3*INST_LENGTH*INST_LENGTH+j*3*INST_LENGTH+k*3] = (GLfloat)k-INST_LENGTH/2;
                cube_rel_pos[i*3*INST_LENGTH*INST_LENGTH+j*3*INST_LENGTH+k*3+1] = (GLfloat)j-INST_LENGTH/2;
                cube_rel_pos[i*3*INST_LENGTH*INST_LENGTH+j*3*INST_LENGTH+k*3+2] = (GLfloat)i-INST_LENGTH;
                //cube_rel_pos[i*3*INST_LENGTH*INST_LENGTH+j*3*INST_LENGTH+k*4+3] = 1;

            }
        }
    }
	/* INST_LENGTH = 3
				i1			   i2			  i3	
		j1 [k1, k2, k3] | [k1, k2, k3] | [k1, k2, k3]
		j2 [k1, k2, k3] | [k1, k2, k3] | [k1, k2, k3]
		j3 [k1, k2, k3] | [k1, k2, k3] | [k1, k2, k3]

	*/
}

void Demo_OGL_4::OnEvent(SDL_Event* event) {
	camera.Event(event);
    switch (event->type) {
        case SDL_KEYUP:
            switch(event->key.keysym.sym){
                case SDLK_v:
                    std::cout << info.client_info() << std::endl << std::endl;
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
void Demo_OGL_4::OnLoop() {
    GLfloat now = (GLfloat)GetTickCount();
    GLfloat elapsedTime = now - lastFrameTime;
	camera.tick(now, elapsedTime);
    lastFrameTime = now;
}

void Demo_OGL_4::OnCleanup() {
    glUseProgram(0);
    glDeleteProgram(render_prog);
    glDeleteVertexArrays(1, vao);
    glDeleteBuffers(2, vbo);
    SDL_GL_DeleteContext(ctxt);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Demo_OGL_4::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    window = SDL_CreateWindow(WIN_TITLE,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WIN_WIDTH, WIN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(!window)
        return false;

    SetupOpenGL();
    InitData();

    running = true;

    return true;
}
void Demo_OGL_4::SetupOpenGL(){

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    ctxt = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(0);

    if (gl3wInit()) {
       std::cout << "Error al Inicializar GL3W" << std::endl;
    }

	if (!gl3wIsSupported(4,3)){
       std::cout << "OpenGL 4.3 no es soportado" << std::endl;
	}
}
int Demo_OGL_4::OnExecute() {
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

void Demo_OGL_4::InitData(){

    ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "../OGL-SDL_Template/app/shaders/geometry_instancingB.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/simple.fs.glsl"  },
        { GL_NONE, NULL }
    };
    // Cargamos los shaders
    render_prog = LoadShaders( shaders );
    // Decimos a opengl que los utilice
    glUseProgram( render_prog );

    // Seleccionamos la posicion dentro del shader de la matriz de modelado y la de proyeccion
    // Mas tarde vamos a usar esas posiciones para cambiar datos del cubo
	render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");

    // Pedimos un buffer para el element buffer object
    glGenBuffers(1, ebo);
    // Le hacemos hueco diciendole el tipo de buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    // Lo rellenamos con los indices de los cubos
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // Pedimos un array de vertices
    glGenVertexArrays(1, vao);
    // Le hacemos hueco
    glBindVertexArray(vao[0]);

    // Pedimos un buffer para el vertex buffer object
    glGenBuffers(2, vbo);
    // Le hacemos hueco
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Le decimos que el hueco tiene que ser de tamaño "tamaño de cube positions"+"tamaño de cube colors"
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions) + sizeof(cube_colors), NULL, GL_STATIC_DRAW);
    // Y como lo tenemos en dos arrays diferentes lo guardamos poco a poco
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_positions), cube_positions );
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_positions), sizeof(cube_colors), cube_colors);

    // localizacion del atributo, numero de valores, tipo de valores,
    // normalizarlos, espacio entre valores, puntero al primer valor.
    // Por lo tanto decimos que nos pille los 4 primeros valores y
    // nos los meta en "posicion",
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    // Y apartir de cube positions que pille los colores.
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(cube_positions));
    // Activamos los dos arrays de atributos
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_rel_pos), cube_rel_pos, GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);


    //Seleccionamos el color de fondo
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    //Los triangulos que no se ven, ni los pinta (los triangulos que estan de espaldas)
    glDisable(GL_CULL_FACE);
    //Utiliza el z buffer
    glEnable(GL_DEPTH_TEST);
    //Funcion para el z-buffer
    glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //lo que ocupa la parte en la que se dibuja.
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	aspect = float(WIN_HEIGHT) / float(WIN_WIDTH); 
	projection_matrix = vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 500.0f);

    CheckErr();
}

void Demo_OGL_4::OnRender() {
    // Un numero incremental en funcion del tiempo real
    float t = float(GetTickCount() & 0x1FFF) / float(0x1FFF);

    // Limpiamos el buffer de profundidad (se pone al valor por defecto)
    // y el de color (se pone al valor de glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Decimos que Shader usar
    glUseProgram(render_prog);

    // Calculamos la matriz modelo
	vmath::mat4 model_matrix(   vmath::rotate(t * 360.0f, Y) *
                                vmath::rotate(t * 720.0f, Z));

    // Posicion en el shader, cantidad de matrices, es traspuesta? , matriz a setear.
    // Guardamos las dos matrices en el shader para que dibuje.
    // Al ser uniform el valor, se aprovecha entre los shaders
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix * camera.matrix());
	// Activamos el vertex array Object
    glBindVertexArray( vao[0] );
    // Activamos el buffer de indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);


#define USE_PRIMITIVE_RESTART 1
#if USE_PRIMITIVE_RESTART
    // When primitive restart is on, we can call one draw command
    // Le decimos que active el reinicio de primitivas,
    glEnable(GL_PRIMITIVE_RESTART);
    // Este es el valor del indice que se toma para el reinicio
    glPrimitiveRestartIndex(0xFFFF);
    // Dibujamos como triangle strip (aprovechamos los dos vertices
    // anteriores para dibujar el siguiente triangulo)
    // un total de 17 indices y de tipo unsigned short sin offset (NULL)
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL, INSTANCES);


#else
    //Dibujamos un strip y otro.
    // Without primitive restart, we need to call two draw commands
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL, INSTANCES);
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT,
            (const GLvoid *)(9 * sizeof(GLushort)), INSTANCES);
#endif
    // Buffer swap
    SDL_GL_SwapWindow(window);
}




void Demo_OGL_4::CheckErr() {
    GLenum err = glGetError();
    if ( err != GL_NO_ERROR )
        std::cerr << "Error: " <<err;
}









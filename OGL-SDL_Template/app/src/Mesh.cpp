
#include "Mesh.h"


Mesh::Mesh() : vertexCount(0), texCoordCount(0), facesCount(0) {}

Mesh* Mesh::load(ifstream& input, GLuint* accVertex) {
	//ifstream input = *in;
	streamoff firstLine = input.tellg();

	Mesh* m = new Mesh();

	string line;
	bool gRead = false;

	while (getline(input, line) && !(line[0] == 'v' && gRead)) {
		switch (line[0]) {
			case 'v':
				switch (line[1]) {
					case ' ': m->vertexCount++; break;
					case 't': m->texCoordCount++; break;
					default:;
				}
			break;
			case 'g': gRead = true;		break;
			case 'f': m->facesCount++;	break;
			default:;
		}
	}

	if (m->vertexCount == 0) return NULL;

	input.clear();
	input.seekg(firstLine);

	m->initializeSizes();

	/******* POPULATING ARRAYS *******/
	GLuint accumulatedVertex = *accVertex;
	*accVertex += m->vertexCount;


	int currentVertex = 0;
	int currentTextCoord = 0;
	int currentFace = 0;
	int facesCount = m->facesCount;

	float x, y, z;
	GLuint xFace, yFace, zFace;
	char auxCString[100];

	while (currentFace < facesCount) {
		getline(input, line);

		switch (line[0]) {
		case 'v': // Vertices & textures
			sscanf(line.c_str(), "%s %f %f %f", &auxCString, &x, &y, &z);
			switch (line[1]) {
			case ' ':
				m->vertex[currentVertex] = Vertex(x, y, z);
				currentVertex++;
				break;
			case 't':
				m->texCoord[currentTextCoord] = TexVertex(x, y);
				currentTextCoord++;
				break;
			default:;
			}
			break;
		case 'g': // Group
			sscanf(line.c_str(), "g %s", &auxCString);
			m->name = string(auxCString);
			cout << m->name << endl; //Mesh_Arid_{A,B, ... , J}_XX de aqui se puede sacar la textura
			break;
		case 'f': // Faces (Triangles)
			sscanf(line.c_str(), "f %f/%u %f/%u %f/%u", &x, &xFace, &y, &yFace, &z, &zFace);
			m->faces[currentFace] = Face(xFace - accumulatedVertex, yFace - accumulatedVertex, zFace - accumulatedVertex);
			currentFace++;
			break;
		default:;
		}
	}

	return m;
}

void Mesh::initOGLData() {

	// Seleccionamos los shaders que queremos cargar
	ShaderInfo shaders1[] = {
		{ GL_VERTEX_SHADER, "../OGL-SDL_Template/app/shaders/texture.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/texture.fs.glsl" },
		{ GL_NONE, NULL }
	};

	render_prog = LoadShaders(shaders1);
	glUseProgram(render_prog);
	render_texture_loc = glGetUniformLocation(render_prog, "texture_diffuse");
	glUniform1i(render_texture_loc, 0); //Texture unit 0 is for base images.
	render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");

	// Pedimos un buffer para el element buffer object
	glGenBuffers(1, ebo);
	// Le hacemos hueco diciendole el tipo de buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	// Lo rellenamos con los indices de los cubos
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfFaces, faces, GL_STATIC_DRAW);

	// Pedimos un array de vertices
	glGenVertexArrays(1, vao);
	// Le hacemos hueco
	glBindVertexArray(vao[0]);

	// Pedimos un buffer para el vertex buffer object
	glGenBuffers(1, vbo);
	// Le hacemos hueco
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// Le decimos que el hueco tiene que ser de tamaño "tamaño de cube positions"+"tamaño de cube colors"
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertex + sizeOfTexCoords, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVertex, vertex);
	glBufferSubData(GL_ARRAY_BUFFER, sizeOfVertex, sizeOfTexCoords, texCoord);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, (const GLvoid*)(sizeOfVertex));


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	texture_A_id = 0;

	// You should probably use CSurface::OnLoad ... ;)
	//-- and make sure the Surface pointer is good!
	texture_A = IMG_Load("../OGL-SDL_Template/app/resources/ObjTex/QuantumArid_Diffuse_A.jpg");

	glActiveTexture(GL_TEXTURE0 + 0);
	glGenTextures(1, &texture_A_id);
	glBindTexture(GL_TEXTURE_2D, texture_A_id);
	glBindSampler(0, GL_LINEAR);

	int mode = GL_RGB;
	if (texture_A->format->BytesPerPixel == 4) mode = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, texture_A->w, texture_A->h, 0, mode, GL_UNSIGNED_BYTE, texture_A->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Mesh::draw(vmath::mat4 model_matrix, vmath::mat4 projection_matrix) {

	// Decimos que Shader usar
	glUseProgram(render_prog);

	// Posicion en el shader, cantidad de matrices, es traspuesta? , matriz a setear.
	// Guardamos las dos matrices en el shader para que dibuje.
	// Al ser uniform el valor, se aprovecha entre los shaders
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);
	// Activamos el vertex array Object
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// Activamos el buffer de indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture_A_id);

	//Dibujamos un strip y otro.
	// Without primitive restart, we need to call two draw commands
	glDrawElements(GL_TRIANGLES, facesCount * 3, GL_UNSIGNED_INT, NULL);

}

#include "Mesh.h"



void Mesh::initOGLData() {

	// Pedimos un buffer para el element buffer object
	_ebo = new GLuint[_eboDataCount];
	_tso = new GLuint[_eboDataCount];
	glGenBuffers(_eboDataCount, _ebo);
	glGenTextures(_eboDataCount, _tso);

	for (GLuint i = 0; i < _eboDataCount; i++) {
		PerDraw *_eboData = &_elementBufferObjectData[i];
		_eboData->indicesSize = _eboData->indicesCount * sizeof(GLuint);
		
		// Le hacemos hueco diciendole el tipo de buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo[i]);
		// Lo rellenamos con los indices de los cubos
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _eboData->indicesSize, _eboData->indices, GL_STATIC_DRAW);

		if (_eboData->mat != nullptr && _eboData->mat->textureDiffuse != "") {
			_texture = IMG_Load(("../OGL-SDL_Template/app/resources/ObjTex/" + _eboData->mat->textureDiffuse).c_str());

			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, _tso[i]);

			int mode = _texture->format->BytesPerPixel == 4? GL_RGBA : GL_RGB;

			glTexImage2D(GL_TEXTURE_2D, 0, mode, _texture->w, _texture->h, 0, 
				mode, GL_UNSIGNED_BYTE, _texture->pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		CheckErr();
	}

	// Pedimos un array de vertices
	glGenVertexArrays(1, _vao);
	// Le hacemos hueco
	glBindVertexArray(_vao[0]);

	// Pedimos un buffer para el vertex buffer object
	glGenBuffers(1, _vbo);
	// Le hacemos hueco
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	_vboDataSize = _vboDataCount * sizeof(PerVertex);
	// Le decimos que el hueco tiene que ser de tamaño "tamaño de cube positions"+"tamaño de cube colors"
	glBufferData(GL_ARRAY_BUFFER, _vboDataSize, _vertexBufferObjectData, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PerVertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PerVertex), (const GLvoid*)sizeof(Vertex));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE,  sizeof(PerVertex), (const GLvoid*)(sizeof(Vertex)+sizeof(Normal)));
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, (const GLvoid*)(_sizeOfVertex));


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	Log::trace("InitOGL Data check...");
	CheckErr();
	Log::trace("CHECKED!");
}

void Mesh::cleanup() {
	/*glDeleteVertexArrays(1, _vao);
	glDeleteBuffers(1, _ebo);
	glDeleteBuffers(1, _vbo);
	glDeleteTextures(1, &_tso);*/
}

void Mesh::draw() {
	if (!_visible) return;

	// Activamos el vertex array Object
	glBindVertexArray(_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);

	CheckErr();
	// Activamos el buffer de indices
	for (GLuint i = 0; i < _eboDataCount; i++) {
		PerDraw *_eboData = &_elementBufferObjectData[i];
		if (_eboData->mat != nullptr && _eboData->mat->textureDiffuse != "") {
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, _tso[i]);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo[i]);
		glDrawElements(GL_TRIANGLES, _elementBufferObjectData[i].indicesCount, GL_UNSIGNED_INT, 0);
		CheckErr();
	}

	CheckErr();
}





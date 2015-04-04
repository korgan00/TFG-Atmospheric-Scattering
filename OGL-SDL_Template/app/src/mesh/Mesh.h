/*
* Mesh.h
*
*  Created on: 16/03/2015
*      Author: Korgan
*/

#ifndef MESH_H_
#define MESH_H_
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include "../../common/common.h"
#include "SDL_image.h"
#include "../Shader.h"
using namespace std;

class Mesh {
public:
	typedef struct {
		Vertex v;
		Normal vn;
		TexVertex vt;
	} PerVertex;

	typedef struct {
		Material *mat;
		GLuint *indices;
		GLuint indicesCount;
		GLuint indicesSize;
	} PerDraw;

	/*typedef vmath::uvec3 Face;
	typedef vmath::vec2 TexVertex;


	//This struct contain 3 floats and a constructor, it's used for vertexes and normal vectors
	struct coordinate{
		float x, y, z;
		coordinate(float a, float b, float c);
	};

	//This structure is store every property of a face
	struct Face{
		int facenum;    //the number of the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it), it's used for the normal vectors
		bool four;              //if true, than it's a quad else it's a triangle
		int faces[4];   //indexes for every vertex, which makes the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it)
		int texcoord[4];        //indexes for every texture coorinate that is in the face (it's start from 1 not 0, so if you use it as an index, subtract 1 from it)
		int mat;                                        //the index for the material, which is used by the face
		face(int facen, int f1, int f2, int f3, int t1, int t2, int t3, int m);        //constuctor for triangle
		face(int facen, int f1, int f2, int f3, int f4, int t1, int t2, int t3, int t4, int m);  //-"- for quad
	};

	//this is a structure, which contain one material
	struct material{
		std::string name;       //the name of the material
		float alpha, ns, ni;      //some property, alpha, shininess, and some other, which we not used
		float dif[3], amb[3], spec[3];    //the color property (diffuse, ambient, specular)
		int illum;      //illum - we not use it
		int texture;    //the id for the texture, if there is no texture than -1
		material(const char* na, float al, float n, float ni2, float* d, float* a, float* s, int i, int t);
	};

	//texture coorinate (UV coordinate), nothing to explain here
	struct texcoord{
		float u, v;
		texcoord(float a, float b);
	};

	*/
private:
	PerVertex *_vertexBufferObjectData;
	GLuint _vboDataCount;
	GLuint _vboDataSize;
	PerDraw *_elementBufferObjectData;
	GLuint _eboDataCount;
	Shader _shader;
	/*
	//std::vector<unsigned int> texture;//the id for all the textures (so we can delete the textures after use it)
	//std::vector<unsigned int> lists;        //the id for all lists (so we can delete the lists after use it)
	//std::vector<material*> materials;       //all materials
	//std::vector<texcoord*> texturecoordinate;       //all texture coorinate (UV coordinate)
	//bool ismaterial, isnormals, istexture;    //obvious
	//unsigned int loadTexture(const char* filename); //private load texture function
	//void clean();   //free all of the used memory

	Vertices _vertices;
	TexVertices _texCoords;
	Normals _normals;
	Faces3v _faces3v;
	Faces4v _faces4v;
	std::vector<Material*> _materials;       //all materials

	/*
	std::string _name;

	SDL_Surface* _texture;
	GLuint _texture_id;
	*/

	SDL_Surface* _texture;
	GLuint _uniformProjectionId;
	GLuint *_ebo;
	GLuint *_tso;
	GLuint _vao[1];
	GLuint _vbo[1];
	
public:

	Mesh(PerVertex* verticesInfo, GLuint verticesCount, PerDraw* drawInfo, GLuint drawCount) :
		_vertexBufferObjectData(verticesInfo), _vboDataCount(verticesCount), 
		_elementBufferObjectData(drawInfo), _eboDataCount(drawCount), _shader() {}

	/*
	void initializeSizes(GLint vertCount, GLint texCount, GLint facesCount) {
		_vertexCount = vertCount;
		_vertices = new Vertex[_vertexCount];
		_sizeOfVertex = _vertexCount * sizeof(Vertex);

		_texCoordCount = texCount;
		_texCoord = new TexVertex[_texCoordCount];
		_sizeOfTexCoords = _texCoordCount * sizeof(TexVertex);

		_facesCount = facesCount;
		_faces = new Face[facesCount];
		_sizeOfFaces = facesCount * sizeof(Face);

		cout << _vertexCount << "::" << _texCoordCount << "::" << _facesCount << endl;
		cout << _sizeOfVertex << ", " << _sizeOfTexCoords << ", " << _sizeOfFaces << endl;
	}


	inline void diffuseTexture(SDL_Surface* tex) { _texture = tex; }

	inline GLint vertexCount() { return _vertexCount; }
	inline Vertex* vertices() { return _vertices; }

	inline GLint texCoordCount() { return _texCoordCount; }
	inline TexVertex* texCoord() { return _texCoord; }

	inline GLint facesCount() { return _facesCount; }
	inline Face* faces() { return _faces; }

	inline void name(string n) { _name = n; }
	inline string name() { return _name; }
	*/

	void draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);

	void initOGLData();
	void cleanup();
};


#endif /* MESH_H_ */

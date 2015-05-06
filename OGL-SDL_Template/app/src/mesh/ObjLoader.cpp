
#include "ObjLoader.h"

string ObjLoader::pathToObjects = "../OGL-SDL_Template/app/resources/";

ObjLoader::ObjFileInfo* ObjLoader::load(string file) {
	ObjFileInfo* obj;
	stringstream ss;
	// OPEN FILE
	ifstream input("../OGL-SDL_Template/app/resources/" + file);
	if (!input.is_open()) {
		ss.str("");
		ss << "Unable to open obj " + file;
		Log::error(ss.str());
		return nullptr;
	}

	vector<NamedObject*> namedObjects;
	Vertices v = { nullptr, 0 };
	Normals vn = { nullptr, 0 };
	TexVertices vt = { nullptr, 0 };
	Materials mtllib = { nullptr, 0 };

	count(input, namedObjects, &(v.count), &(vn.count), &(vt.count), &(mtllib.count));

	input.clear();
	input.seekg(0);

	initializeSizes(v, vn, vt, mtllib);
	initializeNamedObjects(namedObjects);

	fillData(input, v, vn, vt, mtllib, namedObjects);

	input.close();

	//vector<PerDraw*> perDrawData;

	//processPerDrawData(perDrawData, v, vn, vt, mtllib, namedObjects);
	obj = new ObjFileInfo({ v, vn, vt, mtllib, namedObjects });

	return obj;
}

void ObjLoader::count(ifstream& input, vector<NamedObject*> &namedObjects,
	GLuint* vCount, GLuint* vnCount, GLuint* vtCount, GLuint* matCount) {

	char buffer[256];

	namedObjects.push_back(newNamedObject());
	NamedObject* currObj = namedObjects.back();
	ObjectGroup* currGr = nullptr;

	while ( !input.eof() ) {
		input.getline(buffer, 256);
		switch (buffer[0]) {
			case 'v':
				switch (buffer[1]) {
					case ' ': (*vCount)++; break;
					case 't': (*vtCount)++; break;
					case 'n': (*vnCount)++; break;
					default:;
				}
				break;
			case 'f':
				if (currGr == nullptr) currObj->faces.count++;
				else currGr->faces.count++;
				break;
			case 'o':
				namedObjects.push_back(newNamedObject());
				currObj = namedObjects.back();
				currGr = nullptr;
				break;
			case 'g':
				currObj->groups.push_back(newObjectGroup());
				currGr = currObj->groups.back();
				break;
			case 'm':
				if (buffer[1] == 't' && buffer[2] == 'l' &&
					buffer[3] == 'l' && buffer[4] == 'i' && buffer[5] == 'b') {
					countMtllib(buffer, matCount);
				}
				break;
			case 'u':
				if (buffer[1] == 's' && buffer[2] == 'e' &&
					buffer[3] == 'm' && buffer[4] == 't' && buffer[5] == 'l') {
					Log::trace("usemtl skiped on count");
				}
				break;
			case 's':
				Log::debug("s Ignored");
				break;
			default:;
		}
	}

	stringstream ss;
	ss << "v: " << *vCount << " || vn: " << *vnCount << " || vt: " << *vtCount 
		<< " || mtllib: " << *matCount << " || objs: " << namedObjects.size() << endl;

	for (std::vector<NamedObject*>::iterator o = namedObjects.begin(); o != namedObjects.end(); ++o) {
		ss << "[" << (*o)->groups.size() << "], ";
	}
	ss << endl;
	Log::debug(ss.str());
}

void ObjLoader::countMtllib(char line[256], GLuint *matCount) {
	string fileName(line);
	stringstream ss;
	fileName.erase(0, 7);
	ifstream input("../OGL-SDL_Template/app/resources/" + fileName);

	if (!input.is_open()) {
		ss.str("");
		ss << "Unable to open material library " + fileName;
		Log::error(ss.str());
		return;
	}
	char buffer[256];

	while (!input.eof()) {
		input.getline(buffer, 256);

		if (buffer[0] == 'n' && buffer[1] == 'e' && buffer[2] == 'w' &&
			buffer[3] == 'm' && buffer[4] == 't' && buffer[5] == 'l') {
			(*matCount)++;
		}
	}

	input.close();
}

void ObjLoader::initializeSizes(Vertices &v, Normals &vn, TexVertices &vt, Materials &mtllib) {
	v.vertices		 = v.count > 0 ?	  new Vertex[v.count]		 : nullptr;
	vn.normals		 = vn.count > 0 ?	  new Normal[vn.count]		 : nullptr;
	vt.texVertices	 = vt.count > 0 ?	  new TexVertex[vt.count]    : nullptr;
	mtllib.materials = mtllib.count > 0 ? new Material[mtllib.count] : nullptr;

	v.count = 0;
	vn.count = 0;
	vt.count = 0;
	mtllib.count = 0;
}

void ObjLoader::initializeNamedObjects(vector<NamedObject*> &namedObjs) {
	for (std::vector<NamedObject*>::iterator o = namedObjs.begin(); o != namedObjs.end(); ++o) {
		Faces3v* f = &(*o)->faces;
		f->faces = f->count > 0 ? new Face3v[f->count] : nullptr;
		f->count = 0;
		initializeObjectGroups((*o)->groups);
	}
}

void ObjLoader::initializeObjectGroups(vector<ObjectGroup*> &objGroups) {
	for (std::vector<ObjectGroup*>::iterator g = objGroups.begin(); g != objGroups.end(); ++g) {
		Faces3v* f = &(*g)->faces;
		f->faces = new Face3v[f->count];
		f->count = 0;
	}
}

void ObjLoader::fillData(ifstream& input, Vertices &v, Normals &vn, TexVertices &vt,
	Materials &mtllibs, vector<NamedObject*> &namedObjs) {

	char buffer[256];
	string auxName;

	std::vector<NamedObject*>::iterator currObj = namedObjs.begin();
	std::vector<ObjectGroup*>::iterator *currGr = nullptr;
	GLint currMat = -1;

	while (!input.eof()) {
		input.getline(buffer, 256);
		switch (buffer[0]) {
			case 'v': processVertex(buffer, &v, &vn, &vt); break;
			case 'f':
				if (currGr == nullptr) processFace(buffer, &((*currObj)->faces), currMat);
				else processFace(buffer, &((**currGr)->faces), currMat);
				break;
			case 'o':
				++currObj;
				currGr = nullptr;

				auxName = string(buffer);
				auxName.erase(0, 2);
				(*currObj)->name = auxName;
				break;
			case 'g':
				if (currGr != nullptr) ++(*currGr);
				else currGr = &((*currObj)->groups.begin());
				
				auxName = string(buffer);
				auxName.erase(0, 2);
				(**currGr)->name = auxName;
				break;
			case 'm':
				if (buffer[1] == 't' && buffer[2] == 'l' &&
					buffer[3] == 'l' && buffer[4] == 'i' && buffer[5] == 'b') {
					processMtlLib(buffer, mtllibs);
				}
				break;
			case 'u':
				if (buffer[1] == 's' && buffer[2] == 'e' &&
					buffer[3] == 'm' && buffer[4] == 't' && buffer[5] == 'l') {
					auxName = string(buffer);
					auxName.erase(0, 7);
					for (GLuint i = 0; i < mtllibs.count; i++) {
						if (auxName == mtllibs.materials[i].name) {
							currMat = i;
						}
					}
					//Log::trace("usemtl skiped on count");
				}
				break;
			case 's':
				//Log::debug("s Ignored");
				break;
			default:;
		}
	}

	//stringstream ss;
	//ss << "v: " << *vCount << " || vn: " << *vnCount << " || vt: " << *vtCount
		//<< " || mtllib: " << *matCount << " || objs: " << namedObjects.size();
	//Log::debug(ss.str());

}

void ObjLoader::processVertex(char buffer[256], Vertices *v, Normals *vn, TexVertices *vt) {

	switch (buffer[1]) {
		case ' ':
			sscanf(buffer, "v %f %f %f", &(v->vertices[v->count][0]), &(v->vertices[v->count][1]), &(v->vertices[v->count][2]));
			v->count++;
			break;
		case 't':
			sscanf(buffer, "vt %f %f %f", &(vt->texVertices[vt->count][0]), &(vt->texVertices[vt->count][1]), &(vt->texVertices[vt->count][2]));
			vt->count++;
			break;
		case 'n': 
			sscanf(buffer, "vn %f %f %f", &(vn->normals[vn->count][0]), &(vn->normals[vn->count][1]), &(vn->normals[vn->count][2]));
			vn->count++;
			break;
		default:;
	}
}

void ObjLoader::processFace(char buffer[256], Faces3v *f, GLint currentMaterial) {
	GLint v_a = -1, v_b = -1, v_c = -1;
	GLint vt_a = -1, vt_b = -1, vt_c = -1;
	GLint vn_a = -1, vn_b = -1, vn_c = -1;
	string line(buffer);

	if (line.find("//") != std::string::npos) {
		sscanf(buffer, "f %d//%d %d//%d %d//%d", &v_a, &vn_a, &v_b, &vn_b, &v_c, &vn_c);
	} else if (line.find("/") != std::string::npos) {
		if (std::count(line.begin(), line.end(), '/') == 6) {
			sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v_a, &vt_a, &vn_a, &v_b, &vt_b, &vn_b, &v_c, &vt_c, &vn_c);
		} else {
			sscanf(buffer, "f %d/%d %d/%d %d/%d", &v_a, &vt_a, &v_b, &vt_b, &v_c, &vt_c);
		}
	} else{
		sscanf(buffer, "f %d %d %d", &v_a, &v_b, &v_c);
	}


	f->faces[f->count] = { { v_a, v_b, v_c }, { vt_a, vt_b, vt_c }, 
						   { vn_a, vn_b, vn_c }, currentMaterial };
	f->count++;
}

void ObjLoader::processMtlLib(char line[256], Materials &mtllibs) {
	//mtllib Earth.mtl
	string fileName(line);
	fileName.erase(0, 7);
	ifstream input(pathToObjects + fileName);
	stringstream ss;

	if (!input.is_open()) {
		ss.str("");
		ss << "Unable to open material library " + fileName;
		Log::error(ss.str());
		return;
	}

	char buffer[256];
	Material *currMtl;

	while (!input.eof()) {
		input.getline(buffer, 256);
		switch (buffer[0]) {
			case 'n': 
				if (buffer[1] == 'e' && buffer[2] == 'w' &&
					buffer[3] == 'm' && buffer[4] == 't' && buffer[5] == 'l') {
					currMtl = &mtllibs.materials[mtllibs.count];
					currMtl->name = string(buffer);
					currMtl->name.erase(0, 7);
					mtllibs.count++;
				} else {
					ss.str("");
					ss << "Material library: " << line << " has an unknow parameter: " << buffer;
					Log::warning(ss.str());
				}
				break;
			case 'K':
				switch (buffer[1]) {
					case 'a':
						sscanf(buffer, "Ka %f %f %f", &(currMtl->ambient[0]), &(currMtl->ambient[1]), &(currMtl->ambient[2]));
						break;
					case 'd':
						sscanf(buffer, "Kd %f %f %f", &(currMtl->diffuse[0]), &(currMtl->diffuse[1]), &(currMtl->diffuse[2]));
						break;
					case 's':
						sscanf(buffer, "Ks %f %f %f", &(currMtl->specular[0]), &(currMtl->specular[1]), &(currMtl->specular[2]));
						break;
					default:;
				}
				break;
			case 'N':
				switch (buffer[1]) {
					case 's':
						currMtl->specularExponent = (GLfloat)atof(&buffer[3]);
						break;
					case 'i':
						currMtl->opticalDensity = (GLfloat)atof(&buffer[3]);
						break;
					default:;
				}
				break;
			case 'd':
				currMtl->alpha = (GLfloat)atof(&buffer[2]);
				break;
			case 'T':
				if (buffer[1] == 'r') {
					currMtl->alpha = (GLfloat)atof(&buffer[3]);
				} else {
					ss.str("");
					ss << "Material library: " << line << " has an unknow parameter: " << buffer;
					Log::warning(ss.str());
				}
				break;
			case 'i':
				if (buffer[1] == 'l' && buffer[2] == 'l' &&	buffer[3] == 'u' && buffer[4] == 'm') {
					currMtl->illumination = atoi(&buffer[6]);
				} else {
					ss.str("");
					ss << "Material library: " << line << " has an unknow parameter: " << buffer;
					Log::warning(ss.str());
				}
				break;
			case 'm':
				if (buffer[1] == 'a' && buffer[2] == 'p' &&	buffer[3] == '_') {
					switch (buffer[4]) {
						case 'K':
							switch (buffer[5]) {
								case 'd':
									currMtl->textureDiffuse = string(buffer);
									currMtl->textureDiffuse.erase(0, 7);
									break;
								case 'a':
									break;
								case 's':
									break;
								default:;
							}
							break;
						case 'N':
							break;
						case 'd':
							break;
						case 'b':
							if (buffer[5] == 'u' && buffer[6] == 'm' &&	buffer[7] == 'p') {
								currMtl->textureNormalMap = string(buffer);
								currMtl->textureNormalMap.erase(0, 9);
							}
							break;
						default:;
					}
				} else {
					ss.str("");
					ss << "Material library: " << line << " has an unknow parameter: " << buffer;
					Log::warning(ss.str());
				}
				break;
			default:;
		}
	}
	input.close();
}

ObjLoader::NamedObject* ObjLoader::newNamedObject() {
	NamedObject* n = new NamedObject();
	n->faces.count = 0;
	n->groups.clear();
	n->name = "";
	return n;
}

ObjLoader::ObjectGroup* ObjLoader::newObjectGroup() {
	ObjectGroup* g = new ObjectGroup();
	g->faces.count = 0;
	g->name = "";
	return g;
}

/*

typedef struct {
vmath::uvec3 v;
vmath::uvec3 vt;
vmath::uvec3 vn;
GLint material;
} Face3v;
typedef struct {
Face3v *faces;
GLint facesCount;
GLuint sizeOfFaces;
} Faces3v;
*/
/*
void ObjLoader::processPerDrawData(vector<PerDraw*> &perDrawData, Vertices *v, Normals *vn, TexVertices *vt,
	Materials *matlibs, vector<NamedObject*> namedObjs) {

	PerDraw* current = new PerDraw();
	vector<pair<GLint, GLint>> preCount;
	GLint idx = -1;
	for (std::vector<NamedObject*>::iterator o = namedObjs.begin(); o != namedObjs.end(); ++o) {

		preCount.clear();
		preCount.push_back({-1, 0});
		Faces3v currFaces = (*o)->faces;
		for (GLint i = 0; i < currFaces.facesCount; i++) {
			if (currFaces.faces[i].material != preCount.back().first) {
				preCount.push_back({ currFaces.faces[i].material, 1 });
			} else {
				preCount.back().second++;
			}
		}

		std::vector<pair<GLint, GLint>>::iterator preIt = preCount.begin();
		for (GLint i = 0; i < currFaces.facesCount; i++) {
			Face3v face = currFaces.faces[i];
			if (face.material != preIt->first) {
				++preIt;
				PerDraw* current = new PerDraw();
				current->v = new PerVertex[preIt->second * 3];
				current->indices = new GLuint[preIt->second * 3];
				current->vertexCount = current->indicesCount = preIt->second * 3;
				current->mat = matlibs->materials[preIt->first];

				perDrawData.push_back(current);
			}

			PerVertex* perVert_A = new PerVertex();
			PerVertex* perVert_B = new PerVertex();
			PerVertex* perVert_C = new PerVertex();
			// TODO Restar 1?
			perVert_A->v = v->vertices[face.v[0]];
			perVert_A->vn = vn->normals[face.vn[0]];
			perVert_A->vt = vt->texVertices[face.vt[0]];

			perVert_B->v = v->vertices[face.v[1]];
			perVert_B->vn = vn->normals[face.vn[1]];
			perVert_B->vt = vt->texVertices[face.vt[1]];

			perVert_C->v = v->vertices[face.v[2]];
			perVert_C->vn = vn->normals[face.vn[2]];
			perVert_C->vt = vt->texVertices[face.vt[2]];

			current->v[++idx] = *perVert_A;
			current->indices[idx] = idx;
			current->v[++idx] = *perVert_B;
			current->indices[idx] = idx;
			current->v[++idx] = *perVert_C;
			current->indices[idx] = idx;

		}

	}
	//for (std::vector<ObjectGroup*>::iterator g = objGroups.begin(); g != objGroups.end(); ++g) {
}
*/

/*
vector<Mesh> ObjLoader::loadObjects(string file) {

	vector<Mesh> shapes;
	GLuint accumulatedVertex = 1;

	int i = 0;

	// OPEN FILE
	ifstream input("../OGL-SDL_Template/app/resources/" + file);
	if (!input.is_open()) {
		cout << "Unable to open " + file + "\n";
		return shapes;
	}

	Mesh *m;
	while (m = ObjLoader::loadObject(input, &accumulatedVertex)) {
		shapes.push_back(*m);
	}
	
	// Close file
	input.close();

	return shapes;

}

Mesh* ObjLoader::loadObject(ifstream& input, GLuint* accVertex) {
	//ifstream input = *in;
	streamoff firstLine = input.tellg();

	Mesh* m = new Mesh();

	string line;
	bool gRead = false;

	GLint vertexCount = 0;
	GLint texCoordCount = 0;
	GLint facesCount = 0;

	while (getline(input, line) && !(line[0] == 'v' && gRead)) {
		switch (line[0]) {
		case 'v':
			switch (line[1]) {
			case ' ': vertexCount++; break;
			case 't': texCoordCount++; break;
			default:;
			}
			break;
		case 'g': gRead = true;		break;
		case 'f': facesCount++;		break;
		default:;
		}
	}

	if (vertexCount == 0) return NULL;

	input.clear();
	input.seekg(firstLine);

	m->initializeSizes(vertexCount, texCoordCount, facesCount);

	//******* POPULATING ARRAYS *******
	GLuint accumulatedVertex = *accVertex;
	*accVertex += m->vertexCount();


	int currentVertex = 0;
	int currentTextCoord = 0;
	int currentFace = 0;

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
				m->vertices()[currentVertex] = Mesh::Vertex(x, y, z);
				currentVertex++;
				break;
			case 't':
				m->texCoord()[currentTextCoord] = Mesh::TexVertex(x, y);
				currentTextCoord++;
				break;
			default:;
			}
			break;
		case 'g': // Group
			sscanf(line.c_str(), "g %s", &auxCString);
			m->name(string(auxCString));
			cout << m->name() << endl; //Mesh_Arid_{A,B, ... , J}_XX de aqui se puede sacar la textura
			break;
		case 'f': // Faces (Triangles)
			sscanf(line.c_str(), "f %f/%u %f/%u %f/%u", &x, &xFace, &y, &yFace, &z, &zFace);
			m->faces()[currentFace] = Mesh::Face(xFace - accumulatedVertex, yFace - accumulatedVertex, zFace - accumulatedVertex);
			currentFace++;
			break;
		default:;
		}
	}

	return m;
}

*/
/*Mesh* ObjLoader::buildSphere(GLfloat radius, int parallels, int meridians) {
	Mesh* m = new Mesh();

	return m;
}*/


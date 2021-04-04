#ifndef PLANE_H
#define PLANE_H

#include <cmath>
#include <vector>
#include <glm\glm.hpp>
class Plane
{
private:
	int numVertices;
	int numIndices;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	void init(int range, float a, float b, float c,float d);

public:
	Plane();
	Plane(int range,float a,float b, float c,float d);
	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getTangents();
    
}



#endif
#ifndef _SPHERE_NODE_H_
#define _SPHERE_NODE_H_

#include "Node.h"
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class SphereNode : public Node
{
private:
	int mode = 1;
	int modeMem = 0;
	GLuint VAO;
	GLuint VBO, EBO, NBO;
	int stackCount = 40;
	int sectorCount = 40;
	int numsToDraw;
	glm::mat4 model;
	GLuint envmapShader;

	std::list<Node*> children;

public:
	SphereNode(float size, GLuint shader);
	~SphereNode();
	void draw(const glm::mat4& C, GLuint shader);
	void update();
	void addChild(Node* child);
	void toggleMove();
};

#endif

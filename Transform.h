#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"
#include <iostream>
#include <list>

class Transform : public Node
{
private:
	int mode;
	bool moveFlag = true;
	int counter = 0;
	glm::mat4 transform;
	std::list<Node*> children;

public:
	Transform(const glm::mat4& transMatrix, int updateMode);
	~Transform();
	void draw(const glm::mat4& C, GLuint shader);
	void update();
	void addChild(Node* child);
};

#endif

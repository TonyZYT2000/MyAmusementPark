#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Object.h"
#include <vector>
#include <string>
#include <iostream>

class Sphere : public Object
{
private:
	GLuint VAO;
	GLuint VBO, EBO, NBO;
	int stackCount = 40;
	int sectorCount = 40;
	int numsToDraw;

public:
	Sphere(float size);
	~Sphere();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	void spin(float deg);
};

#endif

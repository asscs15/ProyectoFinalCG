#pragma once

#ifndef TOROIDE_H_
#define TOROIDE_H_

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "definition.h"

class Toroide {
public:
	Toroide(float ratio_int, float radio_ext);//slices -> rebanadas
	void init();
	void load();
	void render();
	virtual ~Toroide();
private:
	std::vector<VertexColor> vertexC;
	std::vector<GLuint> index;
	float ratio_int;
	float ratio_ext;
	int slices;
	int stacks;

	GLuint VAO, VBO, EBO;
};

#endif /* TOROIDE_H_ */
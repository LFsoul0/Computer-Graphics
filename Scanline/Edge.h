#pragma once

#include<glm/glm.hpp>

#include"Point.h"

struct Edge
{
	float curr_x, curr_z;
	float dx, dz;
	float max_y;
	glm::vec3 curr_color;
	glm::vec3 dcolor;
	int poly_id;
	Edge* next;

	Edge(float init_x = 0, float init_z = 0, float dx = 0, float dz = 0, float max_y = 0, int poly_id = 0, 
		glm::vec3 init_color = glm::vec3(0), glm::vec3 dcolor = glm::vec3(0), Edge* next = nullptr) 
		: curr_x(init_x), curr_z(init_z), dx(dx), dz(dz), max_y(max_y), poly_id(poly_id), 
		curr_color(init_color), dcolor(dcolor), next(next)
	{

	}

	Edge(const Point& from, const Point& to, int poly_id = 0, Edge* next = nullptr) 
		: poly_id(poly_id), next(next)
	{
		curr_x = from.x;
		curr_z = from.z;
		dx = (to.x - from.x) / (to.y - from.y);
		dz = (to.z - from.z) / (to.y - from.y);
		max_y = to.y;
		curr_color = from.color();
		dcolor = (to.color() - from.color()) / (to.y - from.y);
	}

	~Edge() 
	{

	}
};

struct SubEdge
{
	float curr_z;
	float dz;
	float max_x;
	glm::vec3 curr_color;
	glm::vec3 dcolor;
	SubEdge* next;

	SubEdge(float init_z = 0, float dz = 0, float max_x = 0, 
		glm::vec3 init_color = glm::vec3(0), glm::vec3 dcolor = glm::vec3(0), SubEdge* next = nullptr)
		: curr_z(init_z), dz(dz), max_x(max_x), curr_color(init_color), dcolor(dcolor), next(next)
	{

	}

	SubEdge(const Edge& from, const Edge& to, SubEdge* next = nullptr)
		: next(next)
	{
		curr_z = from.curr_z;
		max_x = to.curr_x;
		curr_color = from.curr_color;

		if (to.curr_x == from.curr_x) {
			dz = 0;
			dcolor = glm::vec3(0);
		}
		else {
			dz = (to.curr_z - from.curr_z) / (to.curr_x - from.curr_x);
			dcolor = (to.curr_color - from.curr_color) / (to.curr_x - from.curr_x);
		}
	}

	~SubEdge()
	{

	}
};

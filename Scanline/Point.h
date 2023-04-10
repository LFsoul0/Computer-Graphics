#pragma once

#include<glm/glm.hpp>

struct Point
{
	float x, y, z;
	float r, g, b;

	Point(float x = 0, float y = 0, float z = 0, float r = 0, float g = 0, float b = 0)
		: x(x), y(y), z(z), r(r), g(g), b(b)
	{

	}

	~Point()
	{

	}

	glm::vec3 pos() const 
	{
		return glm::vec3(x, y, z);
	}

	glm::vec3 color() const 
	{
		return glm::vec3(r, g, b);
	}

	void setPos(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void setColor(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void setPos(glm::vec3 pos)
	{
		this->x = pos.x;
		this->y = pos.y;
		this->z = pos.z;
	}

	void setColor(glm::vec3 color)
	{
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
	}

	void transform(glm::mat4 trans)
	{
		glm::vec4 pos(x, y, z, 1.0f);
		pos = pos * trans;
		x = pos.x;
		y = pos.y;
		z = pos.z;
	}

	void scale(float xscale, float yscale, float zscale = 1.0f)
	{
		x *= xscale;
		y *= yscale;
		z *= zscale;
	}

	void projection(float xoffset, float yoffset)
	{
		x += xoffset * z;
		y += yoffset * z;
	}
};
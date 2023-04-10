#pragma once

#include <glm/glm.hpp>

struct TextureData
{
	size_t width, height;
	float* data;

	TextureData(size_t width, size_t height, glm::vec3 clear_color = glm::vec3(0))
		: width(width), height(height)
	{
		data = new float[3 * width * height]{ 0 };
		if (clear_color != glm::vec3(0)) {
			for (size_t x = 0; x < width; ++x) {
				for (size_t y = 0; y < height; ++y) {
					setPixel(x, y, clear_color);
				}
			}
		}
	}

	~TextureData() {
		delete[] data;
	}

	void setPixel(size_t x, size_t y, glm::vec3& color)
	{
		size_t index = 3 * (width * y + x);
		data[index] = color.r;
		data[index + 1] = color.g;
		data[index + 2] = color.b;
	}
};
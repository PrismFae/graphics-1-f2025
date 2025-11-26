#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <vector>
#include "raymath.h"

// RGBA each channel is 8 bits --> values 0-255
struct Pixel
{
	uint8_t r = 0x0;
	uint8_t g = 0x0;
	uint8_t b = 0x0;
	uint8_t a = 0xFF;
};

struct Texture
{
	int width = -1;
	int height = -1;
	std::vector<Pixel> pixels;
};

void LoadTexture(Texture* texture, int width, int height);

void GenerateGradient(Texture* texture, Vector3 uv_00/*bottom-left*/, Vector3 uv_10/*bottom-right*/, Vector3 uv_01/*top-left*/, Vector3 uv_11/*top-right*/);
void SaveTexture(const char* filename, const Texture& texture);
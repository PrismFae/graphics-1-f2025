#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <vector>

// RGBA each channel is 8 bits --> values 0-255
struct Pixel
{
	uint8_t r = 0x0;
	uint8_t g = 0x0;
	uint8_t b = 0x0;
	uint8_t a = 0xFF;
};

void GenerateGradient(int& x, int& y, std::vector<Pixel>& pixels);
void WriteImage(int x, int y, const std::vector<Pixel>& pixels);
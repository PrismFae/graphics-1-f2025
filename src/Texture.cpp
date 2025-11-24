#include "Texture.h"
#include <cstdint>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

// RGBA each channel is 8 bits --> values 0-255
struct Pixel
{
	uint8_t r = 0xFF;
	uint8_t g = 0xFF;
	uint8_t b = 0xFF;
	uint8_t a = 0xFF;
};

void ImageTest()
{
	int image_width = 512;
	int image_height = 512;
	std::vector<Pixel> pixels;
	pixels.resize(image_width * image_height);
	stbi_write_png("./assets/textures/test.png", image_width, image_height, 4, pixels.data(), 0);
}

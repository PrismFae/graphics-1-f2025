#include "Texture.h"
#include "raymath.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

//#define USE_STB_IMAGE_WRITE
#ifdef USE_STB_IMAGE_WRITE

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

#endif // USE_STB_IMAGE_WRITE

void GenerateGradient(int& image_width, int& image_height, std::vector<Pixel>& pixels)
{
	image_width = 512;
	image_height = 512;
	pixels.resize(image_width * image_height);

	for (int y = 0; y < image_height; y++)
	{
		for (int x = 0; x < image_width; x++)
		{
			Vector2 uv{ x / (float)image_width, y / (float)image_height };
			uv.y = 1.0f - uv.y;

			Pixel& pixel = pixels[y * image_width + x];
			pixel.r = uv.x * 255.0f;
			pixel.g = uv.y * 255.0f;
		}
	}
}

void WriteImage(int x, int y, const std::vector<Pixel>& pixels)
{
#ifdef USE_STB_IMAGE_WRITE
	stbi_write_png("./assets/textures/test.png", x, y, 4, pixels.data(), 0);
#endif
}

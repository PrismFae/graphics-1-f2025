#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

void LoadTexture(Texture* texture, int width, int height)
{
	texture->pixels.resize(width * height);
	texture->width = width;
	texture->height = height;
}

void GenerateGradient(Texture* texture, Vector3 uv_00, Vector3 uv_10, Vector3 uv_01, Vector3 uv_11)
{
	for (int y = 0; y < texture->height; y++)
	{
		for (int x = 0; x < texture->width; x++)
		{
			Vector2 uv{ x / (float)texture->width, y / (float)texture->height };
			uv.y = 1.0f - uv.y;

			Vector3 a = Vector3Lerp(uv_00, uv_10, uv.x);
			Vector3 b = Vector3Lerp(uv_01, uv_11, uv.x);
			Vector3 c = Vector3Lerp(a, b, uv.y);

			Pixel& pixel = texture->pixels[y * texture->width + x];
			pixel.r = c.x * 255.0f;
			pixel.g = c.y * 255.0f;
			pixel.b = c.z * 255.0f;
		}
	}
}

void SaveTexture(const char* filename, const Texture& texture)
{
	stbi_write_png(filename, texture.width, texture.height, 4, texture.pixels.data(), 0);
}

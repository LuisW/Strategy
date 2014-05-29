#include "Texture.h"
#include "ei/glm/glm.hpp"

Texture* BasicLoad(TextureKey key)
{
	SDL_Surface* img = IMG_Load(key.name.c_str());

	if (img != NULL)
	{
		GLuint tex = 0;

		glGenTextures(1, &tex);

		GLenum fmt = GL_RGB;
		GLenum intfmt = GL_RGB8;

		if (img->format->BytesPerPixel == 4)
		{
			fmt = GL_RGBA;
			intfmt = GL_RGBA8;
		}

		float levels = glm::ceil(glm::log2(glm::max(float(img->w), float(img->h)))) - 2;

		glBindTexture(GL_TEXTURE_2D, tex);

		glTexStorage2D(GL_TEXTURE_2D, levels, intfmt, img->w, img->h);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img->w, img->h, fmt, GL_UNSIGNED_BYTE, img->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		GLfloat largest_supported_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);

		return new Texture(tex, img);
	}

	return NULL;
}

Texture* TextureAssetManager::Load(TextureKey key)
{
	return BasicLoad(key);
}

void TextureAssetManager::Unload(AssetIterator it)
{
	SDL_FreeSurface(it->second.data->getImage());

	GLuint tex = it->second.data->getTexture();

	glDeleteTextures(1, &tex);
}
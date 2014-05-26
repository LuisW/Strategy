#pragma once

#include "ei/GL/glew.h"
#include "AssetSubManagerBase.h"
#include "ei/SDL/SDL_image.h"

class Texture
{
private:
	GLuint tex;
	SDL_Surface* img;

	Texture(const Texture&)
	{}

public:
	inline Texture(GLuint _tex, SDL_Surface* surface) : tex(_tex), img(surface)
	{
	}

	inline GLuint getTexture() const
	{
		return tex;
	}

	inline const SDL_Surface* getImage() const
	{
		return img;
	}

	inline SDL_Surface* getImage()
	{
		return img;
	}

	inline unsigned int getWidth() const
	{
		return img->w;
	}

	inline unsigned int getHeight()	const
	{
		return img->h;
	}
};

typedef AssetKey<Texture> TextureKey;
typedef Asset<Texture>	TextureAsset;
typedef const Asset<Texture> TextureAsset_const;

template<>
class AssetKey<Texture>
{
public:
	std::string name;

	std::string toString() const
	{
		return "<Texture>:" + name;
	}

	inline bool operator==(const TextureKey& other) const
	{
		return name == other.name;
	}

	AssetKey(const std::string& _name) : name(_name)
	{

	}
};

namespace std
{
	template <>
	struct hash<TextureKey>
	{
		std::size_t operator()(const TextureKey& k) const
		{
			return hash<std::string>()(k.name);
		}
	};

}

class TextureAssetManager : public AssetSubManagerBase<Texture>
{
private:
	Texture* Load(TextureKey key);
	void Unload(AssetIterator it);
public:
};
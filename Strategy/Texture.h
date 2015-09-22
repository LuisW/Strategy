#pragma once

#include "ei/GL/glew.h"
#include "AssetSubManagerBase.h"
#include "ei/SDL/SDL_image.h"

class Texture
{
private:
	GLuint			m_tex;
	SDL_Surface*	m_pImg;
	unsigned int	m_layers;

	Texture(const Texture&)
	{}

public:
	inline Texture(GLuint tex, SDL_Surface* surface, unsigned int layers = 1) : m_tex(tex), m_pImg(surface), m_layers(layers)
	{
	}

	inline GLuint getTexture() const
	{
		return m_tex;
	}

	inline const SDL_Surface* getImage() const
	{
		return m_pImg;
	}

	inline SDL_Surface* getImage()
	{
		return m_pImg;
	}

	inline unsigned int getWidth() const
	{
		return m_pImg->w;
	}

	inline unsigned int getHeight()	const
	{
		return m_pImg->h;
	}

	inline unsigned int getLayers() const
	{
		return m_layers;
	}
};

typedef AssetKey<Texture> TextureKey;
typedef Asset<Texture>	TextureAsset;
typedef const Asset<Texture> TextureAsset_const;

template<>
class AssetKey<Texture>
{
public:
	std::string m_name;

	std::string toString() const
	{
		return "<Texture>:" + m_name;
	}

	inline bool operator==(const TextureKey& other) const
	{
		return m_name == other.m_name;
	}

	AssetKey(const std::string& _name) : m_name(_name)
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
			return hash<std::string>()(k.m_name);
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
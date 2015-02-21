#pragma once

#include <unordered_map>
#include <stdexcept>
#include "Asset.h"

template<typename T>
class AssetLoadException : std::runtime_error
{
private:
	const AssetKey<T> asset;
	std::string info;

public:
	AssetLoadException(const AssetKey<T> _asset, std::string _info) : std::runtime_error("AssetLoadException"), asset(_asset), info(_info)
	{}

	virtual const char* what() const throw()
	{
		std::string str(std::runtime_error::what());

		str += ": Exception while loading asset " + asset.toString() + ".";

		if (info != "")
		{
			str += " " + info;
		}

		return str.c_str();
	}
};

template<typename T>
class AssetAddException : std::runtime_error
{
private:
	const AssetKey<T> asset;
	std::string info;

public:
	AssetAddException(const AssetKey<T> _asset, std::string _info) : std::runtime_error("AssetAddException"), asset(_asset), info(_info)
	{}

	virtual const char* what() const throw()
	{
		std::string str(std::runtime_error::what());

		str += ": Error adding asset " + asset.toString() + ".";

		if (info != "")
		{
			str += " Details: " + info;
		}

		return str.c_str();
	}
};

template<typename T>
class AssetSubManagerBase
{
private:
	template<typename T>
	struct AssetSubManagerStorage
	{
		ReferenceCounter refCnt;
		T* data;
		bool isPrivate;
	};


protected:
	typedef typename std::unordered_map<AssetKey<T>, AssetSubManagerStorage<T>>::const_iterator AssetIterator;

	std::unordered_map<AssetKey<T>, AssetSubManagerStorage<T>> assets;

	virtual T* Load(AssetKey<T> key) = 0;
	virtual void Unload(AssetIterator it){};

public:

	bool hasAsset(const AssetKey<T>& key)
	{
		auto it = assets.find(key);
		return it != assets.end();
	}

	const Asset<T> GetAsset(const AssetKey<T>& key)
	{
		auto it = assets.find(key);
		if (it != assets.end())
		{
			return Asset<T> (it->second.refCnt, (it->second.data), it->first);
		}
		else
		{
			AssetSubManagerStorage<T> newElem;
			newElem.data = Load(key);

			if (newElem.data == NULL)
			{
				throw AssetLoadException<T>(key, "");
			}

			newElem.refCnt = ReferenceCounter(0);

			assets[key] = newElem;

			it = assets.find(key);
			return Asset<T>(it->second.refCnt, (it->second.data), it->first);
		}
	}

	Asset<T> GetPrivateAsset(AssetKey<T> key);

	const Asset<T> AddAsset(const AssetKey<T>& key, T* asset)
	{
		auto it = assets.find(key);
		if (it != assets.end())
		{
			throw AssetAddException<T>(key, "key already in use.");
		}

		AssetSubManagerStorage<T> newElem;
		newElem.data = asset;

		if (newElem.data == NULL)
		{
			throw AssetAddException<T>(key, "NULL asset.");
		}

		newElem.refCnt = ReferenceCounter(0);

		assets[key] = newElem;

		it = assets.find(key);
		return Asset<T>(it->second.refCnt, (it->second.data), it->first);
	}
	
	void CleanUnused()
	{
		auto it = assets.begin();

		while (it != assets.end())
		{
			if (!it->second.refCnt.isReferenced())
			{
				Unload(it);
				delete it->second.data;
				assets.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}
	
	void DeleteAll()
	{
		auto it = assets.begin();

		while (it != assets.end())
		{
			Unload(it);
			delete it->second.data;
			assets.erase(it++);
		}
	}

	~AssetSubManagerBase()
	{
	}
};
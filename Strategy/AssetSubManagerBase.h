#pragma once

#include <unordered_map>
#include "Asset.h"

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
			newElem.refCnt = ReferenceCounter(0);

			assets[key] = newElem;

			it = assets.find(key);
			return Asset<T>(it->second.refCnt, (it->second.data), it->first);
		}
	}

	Asset<T> GetPrivateAsset(AssetKey<T> key);
	void AddAsset(AssetKey<T> key, T& asset);
	
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
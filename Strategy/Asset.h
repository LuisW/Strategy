#pragma once

#include "ReferenceCounter.h"


template<typename T> class AssetKey;

//template< typename T >
//class TAssetManager
//{
//private:
//	std::unordered_map<AssetKey<T>, T> assets;
//
//public:
//	Asset<T>& GetAsset(AssetKey<T> key);
//	const Asset<T>& GetPrivateAsset(AssetKey<T> key);
//	Asset<T> AddAsset(AssetKey<T> key, T& asset);
//};

template<typename T>
class Asset
{
private:
	ReferenceCounter& refCnt;
	T* data;
	const AssetKey<T>& key;

public:
	Asset(ReferenceCounter& _refCnt, T* _data, const AssetKey<T>& _key) : refCnt(_refCnt), data(_data), key(_key)
	{
		refCnt.AddReference();
	}

	Asset(const Asset& other) : refCnt(other.refCnt), data(other.data), key(other.key)
	{
		refCnt.AddReference();
	}

	const Asset& operator=(const Asset& rhs)
	{
		refCnt.DeleteReference();
		refCnt = rhs.refCnt;
		data = rhs.data;
		key = rhs.key;
		refCnt.AddReference();
	}

	const T& get() const
	{
		return *data;
	}

	T& get()
	{
		return *data;
	}

	const AssetKey<T>& getKey() const
	{
		return key;
	}

	~Asset()
	{
		refCnt.DeleteReference();
	}
};
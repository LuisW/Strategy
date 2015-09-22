#pragma once

#include "ReferenceCounter.h"


template<typename T> class AssetKey
{
	std::string toString() const
	{
		return "Unknown";
	}
};

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
	ReferenceCounter&	m_refCnt;
	const AssetKey<T>&	m_key;
	T*					m_pData;

public:
	Asset(ReferenceCounter& refCnt, T* data, const AssetKey<T>& key) : m_refCnt(refCnt), m_pData(data), m_key(key)
	{
		m_refCnt.AddReference();
	}

	Asset(const Asset& other) : m_refCnt(other.m_refCnt), m_pData(other.m_pData), m_key(other.m_key)
	{
		m_refCnt.AddReference();
	}

	const Asset& operator=(const Asset& rhs)
	{
		m_refCnt.DeleteReference();
		m_refCnt = rhs.m_refCnt;
		m_pData = rhs.m_pData;
		m_key = rhs.m_key;
		m_refCnt.AddReference();
	}

	const T& get() const
	{
		return *m_pData;
	}

	T& get()
	{
		return *m_pData;
	}

	const AssetKey<T>& getKey() const
	{
		return m_key;
	}

	~Asset()
	{
		m_refCnt.DeleteReference();
	}
};
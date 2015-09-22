#pragma once

#include <unordered_map>
#include <stdexcept>
#include "Asset.h"
#include "DebugHelp.h"

template<typename T>
class AssetLoadException : std::runtime_error
{
private:
	std::string m_msg;

public:
	AssetLoadException(const AssetKey<T> asset, std::string info) : std::runtime_error("AssetLoadException"), m_msg(std::runtime_error::what())
	{
		m_msg += ": Exception while loading asset " + asset.toString() + ".";

		if (info != "")
		{
			m_msg += " " + info;
		}
	}

	const char* what() const throw() override
	{
		return m_msg.c_str();
	}
};

template<typename T>
class AssetAddException : std::runtime_error
{
private:
	std::string m_msg;

public:
	AssetAddException(const AssetKey<T> asset, std::string info) : std::runtime_error("AssetAddException"), m_msg(std::runtime_error::what())
	{
		m_msg += ": Error adding asset " + asset.toString() + ".";

		if (info != "")
		{
			m_msg += " Details: " + info;
		}
	}

	const char* what() const throw() override
	{
		return m_msg.c_str();
	}
};

template<typename T>
class AssetSubManagerBase
{
private:
	template<typename T>
	struct AssetSubManagerStorage
	{
		ReferenceCounter	m_refCnt;
		T*					m_pData;
		bool				m_isPrivate;
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
			return Asset<T> (it->second.m_refCnt, (it->second.m_pData), it->first);
		}
		else
		{
			AssetSubManagerStorage<T> newElem;
			try
			{
				newElem.m_pData = Load(key);
			}
			catch (AssetLoadException<T>& ex)
			{
				ERROR(ex.what())
			}

			if (newElem.m_pData == NULL)
			{
				throw AssetLoadException<T>(key, "");
			}

			newElem.m_refCnt = ReferenceCounter(0);

			assets[key] = newElem;

			it = assets.find(key);
			return Asset<T>(it->second.m_refCnt, (it->second.m_pData), it->first);
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
		newElem.m_pData = asset;

		if (newElem.m_pData == NULL)
		{
			throw AssetAddException<T>(key, "NULL asset.");
		}

		newElem.m_refCnt = ReferenceCounter(0);

		assets[key] = newElem;

		it = assets.find(key);
		return Asset<T>(it->second.m_refCnt, (it->second.m_pData), it->first);
	}
	
	void CleanUnused()
	{
		auto it = assets.begin();

		while (it != assets.end())
		{
			if (!it->second.refCnt.isReferenced())
			{
				Unload(it);
				delete it->second.m_pData;
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
			delete it->second.m_pData;
			assets.erase(it++);
		}
	}

	~AssetSubManagerBase()
	{
	}
};
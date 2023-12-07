#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore\Sporepedia\AssetData.h>

#define AssetTypeManagerPtr intrusive_ptr<AssetTypeManager>

class AssetTypeManager 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("AssetTypeManager");
	
	AssetTypeManager();
	~AssetTypeManager();

	static PropertyListPtr GetAssetType(uint32_t id);
	
	static bool AssetTypeManager::HasAssetType(uint32_t identifier);
	static const char16_t* GetName(uint32_t identifier);
	static uint32_t AssetTypeManager::GetSourceType(uint32_t identifier);
	static uint32_t AssetTypeManager::GetTypeEditor(uint32_t identifier);
	static bool AssetTypeManager::GetIsSharable(uint32_t identifier);

	static void AttachDetours();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	static uint32_t ActiveType;

private:
	static hash_map<uint32_t, PropertyListPtr> mpTypeMap;
	
};

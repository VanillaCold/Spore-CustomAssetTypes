#include "stdafx.h"
#include "AssetTypeManager.h"

AssetTypeManager::AssetTypeManager()
{
}


AssetTypeManager::~AssetTypeManager()
{
}

// For internal use, do not modify.
int AssetTypeManager::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int AssetTypeManager::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* AssetTypeManager::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(AssetTypeManager);
	return nullptr;
}

void AssetTypeManager::AttachDetours()
{
	///Note: RIGHT NOW, ADDRESSES ARE EXACT, rather than choosing for different executables.
	///This means that this mod won't work with Disk Spore for now.

	EditorEntryDetour::attach(Address(0x004333e0)); //TODO: Find the address for Disk Spore

	TypeDetour::attach(Address(0x004bbda0)); //TODO: Find the address for Disk Spore

	BackgroundImageDetour::attach(GetAddress(Sporepedia::cSPAssetDataOTDB, GetBackgroundImageKey));

	//and also just find the actual functions and class, so that it can be added to the SDK.

	TypeNameDetour::attach(Address(0x004bba50));


	//Makes every type editable. Useful if a type is a flora derivative.
	EditAllCreationsDetour::attach(GetAddress(Sporepedia::cSPAssetDataOTDB, IsEditable));
}

// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Spore\Sporepedia\ObjectTemplateDB.h>
#include <Spore\Sporepedia\AssetData.h>
#include "AssetTypeManager.h"

void Initialize()
{
	vector<uint32_t> IDs = { id("Home"), id("editable"), id("UserCreated"), id("UserPollinated"), 0x976CCB9A};

	for (int i = 0; i < IDs.size(); i++)
	{
		PropertyListPtr propList;
		if (PropManager.GetPropertyList(IDs[i], id("AssetBrowserFilter"), propList))
		{
			//CAT_CustomTypes
			size_t count;
			ResourceKey* key;
			App::Property::GetArrayKey(propList.get(), 0x7435A2D3, count, key);

			ResourceKey* keyArray = new ResourceKey[count + 1]{};
			for (int j = 0; j < count; j++)
			{
				keyArray[j] = key[j];
			}
			keyArray[count] = ResourceKey(id("CAT_CustomTypes"), 0, 0);

			App::Property* prop;

		
			propList->GetProperty(0x7435A2D3, prop);
			prop->SetArrayKey(keyArray,count+1);

			bool ownsMemory = (prop->mnFlags & 0x20) != 0x20;
			if (!ownsMemory)
			{
				prop->mnFlags = static_cast<short>((prop->mnFlags & ~0x20) | 0x4 | 0x10);
			}
			else
			{
				delete [] key;
			}
		}
	}
	//Home


	new AssetTypeManager();
}

void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	AssetTypeManager::AttachDetours();
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

/*virtual_detour(WhatAmIDoing, Sporepedia::cSPAssetDataOTDB, Sporepedia::IAssetData, uint32_t())
{
	uint32_t detoured()
	{
		if (this->TYPE == Sporepedia::cSPAssetDataOTDB::TYPE)
		{
			auto aaa = this->mSubtype;
			if (this->mSubtype == id("TestTEST"))
			{
				this->mSubtype = id("BuildingHouse");
				return(id("BuildingHouse"));
			}
			return original_function(this);
		}
		else
		{
			return original_function(this);
		}
	}
};*/
/*virtual_detour(BackgroundImageDetour, Sporepedia::cSPAssetDataOTDB, Sporepedia::IAssetData, void(ResourceKey& a))
{
	void detoured(ResourceKey& a)
	{
		if (this)
		{
			uint32_t aid = this->mSubtype;
			if (aid == id("TestTEST"))
			{
				this->mIsViewableLarge = true;//mSubtype = id("VehicleMilitaryLand");
			}
			original_function(this,a);
			//this->mSubtype = aid;
		}
		return original_function(this,a);
	}

};*/
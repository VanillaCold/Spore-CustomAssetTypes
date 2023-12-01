// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Spore\Sporepedia\ObjectTemplateDB.h>
#include <Spore\Sporepedia\AssetData.h>
#include "AssetTypeManager.h"

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
	//ManualBreakpoint();


	//SporeDebugPrint("%x", (uint32_t)GetAddress(Sporepedia::cSPAssetDataOTDB, GetBackgroundImageKey));
	//SporeDebugPrint("%x", (uint32_t)(-0x4328c177));
	
	//App::ConsolePrintF("%x",baseAddress);
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
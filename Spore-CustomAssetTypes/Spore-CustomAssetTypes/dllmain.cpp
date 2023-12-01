// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Spore\Sporepedia\ObjectTemplateDB.h>
#include <Spore\Sporepedia\AssetData.h>

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
}

void Dispose()
{
	// This method is called when the game is closing
}

virtual_detour(BackgroundImageDetour, Sporepedia::cSPAssetDataOTDB, Sporepedia::IAssetData, void(ResourceKey& a))
{
	void detoured(ResourceKey & a)
	{
		if (this)
		{
			uint32_t aid = this->mSubtype;
			if (aid == id("TestTEST"))
			{
				original_function(this, a);
				//a = ResourceKey(0xA518147D, TypeIDs::png, id("AssetBrowserGraphics"));
				a.instanceID = 0xA518147D;
				return;
			}
			return original_function(this, a);
			//this->mSubtype = aid;
		}
		return original_function(this, a);
	}

};


static_detour(TypeDetour, int(uint32_t)) //Detour what type the game things yours actually is.
{
	//TODO: Make this data-driven. Like, 100% data-driven.
	int detoured(uint32_t type)
	{
		uint32_t tType = type;
		if (type == id("TestTEST"))
		{
			tType = id("VehicleMilitaryLand");
		}
		return original_function(tType);
	}
};

virtual_detour(EditAllCreationsDetour, Sporepedia::cSPAssetDataOTDB, Sporepedia::IAssetData, bool())
{
	bool detoured()
	{
		//Make it so that the game always allows creations to be edited.
		original_function(this);
		return true;
	}
};

static_detour(EditorEntryDetour, int(uint32_t)) //Detour what editor the game puts your creation in.
{
	//TODO: Make this entirely data-driven!
	int detoured(uint32_t edID)
	{
		if (edID == id("TestTEST"))
		{
			return id("TestTEST");
		}
		return original_function(edID);
	}
};

void AttachDetours()
{
	///Note: RIGHT NOW, ADDRESSES ARE EXACT, rather than choosing for different executables.
	///This means that this mod won't work with Disk Spore for now.

	EditorEntryDetour::attach(Address(0x004333e0)); //TODO: Find the address for Disk Spore

	TypeDetour::attach(Address(0x004bbda0)); //TODO: Find the address for Disk Spore

	BackgroundImageDetour::attach(GetAddress(Sporepedia::cSPAssetDataOTDB, GetBackgroundImageKey));

	//and also just find the actual functions and class, so that it can be added to the SDK.
	

	//Makes every type editable. Useful if a type is a flora derivative.
	EditAllCreationsDetour::attach(GetAddress(Sporepedia::cSPAssetDataOTDB, IsEditable));
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
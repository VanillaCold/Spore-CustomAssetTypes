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

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

	static void AttachDetours();





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

	static_detour(TypeNameDetour, wchar_t(uint32_t))
	{
		wchar_t detoured(uint32_t type)
		{
			if (type == id("TestTEST"))
			{
				return wchar_t(u"TestTEST");
			}
			return original_function(type);
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

	
};

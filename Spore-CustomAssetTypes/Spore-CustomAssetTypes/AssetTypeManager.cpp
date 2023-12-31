#include "stdafx.h"
#include "AssetTypeManager.h"
#include <Spore\Hash.h>
#include <Spore/GeneralAllocator.h>

#include <Spore/App/cPropManager.h>


AssetTypeManager::AssetTypeManager()
{
	mpTypeMap = hash_map<uint32_t, PropertyListPtr>();
	vector<uint32_t> result;
	PropManager.GetPropertyListIDs(id("CustomAssetTypes"), result);
	for each (uint32_t ID in result)
	{
		PropertyListPtr propList;
		PropManager.GetPropertyList(ID, id("CustomAssetTypes"), propList);
		mpTypeMap.emplace(ID, propList);
	}
	ActiveType = 0;
}


AssetTypeManager::~AssetTypeManager()
{
}

bool AssetTypeManager::HasAssetType(uint32_t identifier)
{
	if (mpTypeMap.find(identifier) != mpTypeMap.end())
	{
		return true;
	}
	return false;
}

const char16_t* AssetTypeManager::GetName(uint32_t identifier)
{
	LocalizedString str;
	PropertyListPtr propList;
	PropManager.GetPropertyList(identifier, id("CustomAssetTypes"), propList);
	App::Property::GetText(propList.get(), id("assetTypeName"), str);
	
	return str.GetText();
}

uint32_t AssetTypeManager::GetSourceType(uint32_t identifier)
{
	ResourceKey typeID;
	PropertyListPtr propList;
	PropManager.GetPropertyList(identifier, id("CustomAssetTypes"), propList);
	App::Property::GetKey(propList.get(), id("parentAssetType"), typeID);

	return typeID.instanceID;
}

uint32_t AssetTypeManager::GetTypeEditor(uint32_t identifier)
{
	ResourceKey editorID;
	PropertyListPtr propList;
	PropManager.GetPropertyList(identifier, id("CustomAssetTypes"), propList);
	App::Property::GetKey(propList.get(), id("editorToUse"), editorID);

	return editorID.instanceID;
}

bool AssetTypeManager::GetIsSharable(uint32_t identifier)
{
	bool canShare;
	PropertyListPtr propList;
	PropManager.GetPropertyList(identifier, id("CustomAssetTypes"), propList);
	
	if (App::Property::GetBool(propList.get(), id("assetTypeSharable"), canShare))
	{
		return canShare;
	}
	
	return false;
}

PropertyListPtr AssetTypeManager::GetAssetType(uint32_t identifier)
{
	if (mpTypeMap.find(identifier) != mpTypeMap.end())
	{
		return mpTypeMap[identifier];
	}
	return nullptr;
}




virtual_detour(IsSharableDetour, Sporepedia::cSPAssetDataOTDB, Sporepedia::IAssetData, bool())
{
	bool detoured()
	{
		if (this)
		{
			if (AssetTypeManager::HasAssetType((uint32_t)this->GetAssetSubtype()))
			{
				return AssetTypeManager::GetIsSharable( (uint32_t)this->GetAssetSubtype() );
			}
		}
		return original_function(this);
	}
};

virtual_detour(BackgroundImageDetour, Sporepedia::cSPAssetDataOTDB, Sporepedia::IAssetData, ResourceKey*(ResourceKey* key))
{

	ResourceKey* detoured(ResourceKey * key)
	{
		if (this)
		{
			if (AssetTypeManager::HasAssetType((uint32_t)this->GetAssetSubtype()))
			{
				uint32_t ID = 0xA518147D;
				if (App::Property::GetUInt32(AssetTypeManager::GetAssetType((uint32_t)this->GetAssetSubtype()).get(), id("assetTypeBackgroundID"), ID))
				{ 
					ResourceKey* ret = new ResourceKey(0,0,0);
					ret->instanceID = ID;
					ret->groupID = 0xCA14DE92;
					ret->typeID = TypeIDs::png;
					key = ret;
					return key;
				}
			}
			else
			{
				return original_function(this,key);
			}

		}
		return original_function(this,key);
	}

};

static_detour(TypeNameDetour, const char16_t* (uint32_t))
{
	const char16_t* detoured(uint32_t type)
	{
		if (AssetTypeManager::HasAssetType(type))
		{
			return AssetTypeManager::GetName(type);
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
		if (AssetTypeManager::HasAssetType(type))
		{
			tType = AssetTypeManager::GetSourceType(type);
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
	int detoured(uint32_t edID)
	{
		if (AssetTypeManager::HasAssetType(edID))
		{
			return AssetTypeManager::GetTypeEditor(edID);
		}
		return original_function(edID);
	}
};
uint32_t AssetTypeManager::ActiveType;

member_detour(UpdateDetour, Editors::cEditor, void(float,float))
{

	///TODO: Clean up code and add comments
	///Also, add custom icon for these creations.

	void detoured(float delta1, float delta2)
	{
		if (AssetTypeManager::GetAssetType(mpEditorModel->mModelType))
		{
			auto test = mEditorName;
			auto currentType = mpEditorModel->mModelType;
			AssetTypeManager::ActiveType = currentType;

			uint32_t b;
			App::Property::GetUInt32(AssetTypeManager::GetAssetType(currentType).get(), id("editorPretendType"), b);

			mpEditorModel->mModelType = b;
			
			ResourceKey a;
			App::Property::GetKey(AssetTypeManager::GetAssetType(currentType).get(), id("editorParentEditor"), a);

			mEditorName = a.instanceID;//CALL(Address(0x004333e0), uint32_t, Args(int), Args(mpEditorModel->mModelType));
			original_function(this,delta1,delta2);
			
			mEditorName = test;
			mpEditorModel->mModelType = currentType;
			
			AssetTypeManager::ActiveType = 0;

			return;
		}
		AssetTypeManager::ActiveType = 0;
		return original_function(this,delta1,delta2);
	}

};

member_detour(PropManagerDetour, App::cPropManager, bool(uint32_t, uint32_t, PropertyListPtr&))
{
	bool detoured(uint32_t instanceID, uint32_t groupID, PropertyListPtr& output)
	{
		if (AssetTypeManager::ActiveType != 0 && groupID == 0x01B68DB4)
		{
			uint32_t type = AssetTypeManager::ActiveType;
			PropertyListPtr propList = AssetTypeManager::GetAssetType(type);
			uint32_t overrideID;

			if (App::Property::GetUInt32(propList.get(), id("editorOverrideVerbtray"), overrideID) && instanceID == overrideID)
			{
				bool canUse = false;
				if (App::Property::GetBool(AssetTypeManager::GetAssetType(type).get(), id("useCustomVerbtray"), canUse) && canUse)
				{
					instanceID = type;
				}
			}
		}
		return original_function(this, instanceID, groupID, output);
	}
};

/*static_detour(ParameterDetour, void(int*, uint32_t))
{
	void detoured(int* a, uint32_t b)
	{
		original_function(a,b);
		if (AssetTypeManager::HasAssetType(b))
		{
			uint32_t ID;
			if (App::Property::GetUInt32(AssetTypeManager::GetAssetType(b).get(), id("assetTypeBackgroundID"), ID))
			{
				*a = ID;
				return;
			}
			//
		}
		//ModAPI::Log("%b", z);
	}

};*/

void AssetTypeManager::AttachDetours()
{	
	PropManagerDetour::attach(GetAddress(App::cPropManager, GetPropertyList));

	//For each of these, the first address is disk address, and the latter is the March2017 address.
	EditorEntryDetour::attach(Address(ModAPI::ChooseAddress(0x00433010, 0x004333e0)));

	TypeNameDetour::attach(Address(ModAPI::ChooseAddress(0x004badc0, 0x004bba50)));
	
	TypeDetour::attach(Address(ModAPI::ChooseAddress(0x004bb110, 0x004bbda0)));

	BackgroundImageDetour::attach(GetAddress(Sporepedia::cSPAssetDataOTDB, GetBackgroundImageKey));

	IsSharableDetour::attach(GetAddress(Sporepedia::cSPAssetDataOTDB, IsShareable));

	UpdateDetour::attach(GetAddress(Editors::cEditor, Update));//Address(0x00407280));

	//TestDetour::attach(Address(0x04e7a00));

	//FUN_00576140
	//FUN_00407280

	//Makes every type editable. Useful if a type is a flora derivative.
	EditAllCreationsDetour::attach(GetAddress(Sporepedia::cSPAssetDataOTDB, IsEditable));
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

hash_map<uint32_t, PropertyListPtr> AssetTypeManager::mpTypeMap;
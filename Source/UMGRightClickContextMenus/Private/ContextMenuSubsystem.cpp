// Author: Tom Werner (dc: majort), 2026


#include "ContextMenuSubsystem.h"

#include "ContextMenuLog.h"
#include "GameplayTagContainer.h"

void FRCContextMenuHandle::Unregister()
{
	if (UContextMenuSubsystem* Sub = MenuSubsystem.Get())
	{
		Sub->UnregisterRCContextMenu(*this);
	}
}

void FRCContextMenuActionHandle::Unregister()
{
	if (UContextMenuSubsystem* Sub = MenuSubsystem.Get())
	{
		Sub->UnregisterRCContextMenuAction(*this);
	}
}

UContextMenuSubsystem* UContextMenuSubsystem::Get(const UObject* ContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (IsValid(World))
	{
		return World->GetSubsystem<UContextMenuSubsystem>();
	}
	
	return nullptr;
}

/*FGameplayTag UContextMenuSubsystem::GetMenuActionTag(const UUserWidget* Widget) const
{
	if (auto* Tag = ActionWidgetsMap.Find(Widget))
	{
		return *Tag;
	}
	
	return FGameplayTag();
}*/

void UContextMenuSubsystem::ExecuteMenuAction(const FGameplayTag& ActionTag)
{
}

FRCContextMenuHandle UContextMenuSubsystem::RegisterRCContextMenu(
	const FGameplayTag& MenuTag,
	FRegisterMenuActionDelegate RegisterMenuActionCallback)
{
	if (!MenuTag.IsValid())
	{
		UE_LOG(LogContextMenus, Warning, TEXT("Trying to register a context menu anchor with an invalid anchor tag."))
		return FRCContextMenuHandle();
	}
	
	if (!RegisterMenuActionCallback.IsBound())
	{
		UE_LOG(LogContextMenus, Warning, TEXT("Trying to register a context menu anchor (%s) with an invalid callback."),
			*MenuTag.ToString())
		return FRCContextMenuHandle();
	}
	
	
	TSharedPtr<FRCContextMenu>& Entry = MenuList.Add_GetRef(MakeShared<FRCContextMenu>());
	Entry->MenuTag = MenuTag;
	Entry->Callback = MoveTemp(RegisterMenuActionCallback);
	
	// Using generation to allow multiple menus with the same tag
	// For instance in an inventory, each slot will have a context menu of the exact same type
	static int32 GenIdx = 0;
	Entry->Generation = ++GenIdx;
	
	UE_LOG(LogContextMenus, Verbose, TEXT("Context menu anchor (%s) registered!"),
		*MenuTag.ToString());
	
	NotifyMenuOfActions(Entry);
	
	return FRCContextMenuHandle(this, Entry);
}

void UContextMenuSubsystem::UnregisterRCContextMenu(const FRCContextMenuHandle& MenuHandle)
{
}

FRCContextMenuActionHandle UContextMenuSubsystem::RegisterRCContextMenuAction(
	FRCContextMenuHandle Menu,
	FRCContextMenuActionDescriptor Action)
{
	if (!Menu.IsValid())
	{
		UE_LOG(LogContextMenus, Warning, TEXT("Trying to register a context menu action with an invalid menu."))
		return FRCContextMenuActionHandle();
	}
	
	/*if (ActionsPerMenu.Contains(Menu))
	{
		UE_LOG(LogContextMenus, Warning, TEXT("Trying to register a context menu action (%s) which has already been registered."),
			*Action.ActionTag.ToString())
		return FRCContextMenuActionHandle();
	}*/
	
	FActionMap& List = ActionsPerMenu.FindOrAdd(Menu);
	
	// Check for dupes
	for (auto& Entry : List)
	{
		if (Entry.Key.MatchesTagExact(Action.ActionTag))
		{
			UE_LOG(LogContextMenus, Warning, TEXT("Trying to register a context menu action (%s) which has already been registered."),
				*Action.ActionTag.ToString())
			
			return FRCContextMenuActionHandle();
		}
	}
	
	auto& Entry = List.Add(Action.ActionTag, MakeShared<FRCContextMenuAction>());
	
	Entry->ActionTag = Action.ActionTag;
	Entry->MenuHandle = Menu;
	Entry->Priority = Action.Priority;
	Entry->DisplayName = Action.DisplayName;
	Entry->ExecuteCallback = MoveTemp(Action.ExecuteActionCallback);
	
	NotifyActionOfMenus(true, Entry);
	
	return FRCContextMenuActionHandle(this, Entry);
}

void UContextMenuSubsystem::UnregisterRCContextMenuAction(const FRCContextMenuActionHandle& ActionHandle)
{
}

void UContextMenuSubsystem::ExecuteMenuAction(const FRCContextMenuActionHandle& ActionHandle)
{
	if (!ActionHandle.IsValid())
	{
		return;
	}
	
	ActionHandle.DataPtr->ExecuteCallback.ExecuteIfBound(ActionHandle.DataPtr->ActionTag);
}

void UContextMenuSubsystem::NotifyMenuOfActions(const FRCContextMenuHandle& Menu)
{
	NotifyMenuOfActions(Menu.DataPtr);
}


void UContextMenuSubsystem::NotifyMenuOfActions(const TSharedPtr<FRCContextMenu>& Menu)
{
	for (auto& Pair : ActionsPerMenu)
	{
		if (Pair.Key.DataPtr.Get() != Menu.Get())
		{
			continue;
		}
		
		auto& Actions = Pair.Value;
		for (auto& Action : Actions)
		{
			auto Request = CreateActionDescriptor(Action.Value);
			Request.MenuHandle = FRCContextMenuHandle(this, Menu);
			
			Menu->Callback.ExecuteIfBound(true, Request);
		}
		break;
	}
}

void UContextMenuSubsystem::NotifyActionOfMenus(
	bool bIsAdd,
	const TSharedPtr<FRCContextMenuAction>& Action)
{
	for (auto& MyMenu : MenuList)
	{
		if (Action->MenuHandle.DataPtr.Get() != MyMenu.Get())
		{
			continue;
		}
		
		auto Request = CreateActionDescriptor(Action);
		Request.MenuHandle = FRCContextMenuHandle(this, MyMenu);
		
		MyMenu->Callback.ExecuteIfBound(bIsAdd, Request);
	}
}

FRCContextMenuActionDescriptor UContextMenuSubsystem::CreateActionDescriptor(
	const TSharedPtr<FRCContextMenuAction>& Action)
{
	FRCContextMenuActionDescriptor Descriptor;
	Descriptor.ActionHandle = FRCContextMenuActionHandle(this, Action);
	Descriptor.ActionTag = Action->ActionTag;
	Descriptor.Priority = Action->Priority;
	Descriptor.DisplayName = Action->DisplayName;
	Descriptor.ExecuteActionCallback = Action->ExecuteCallback;
	
	return Descriptor;
}

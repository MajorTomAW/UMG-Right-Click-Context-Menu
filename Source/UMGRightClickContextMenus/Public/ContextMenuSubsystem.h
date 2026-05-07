// Author: Tom Werner (dc: majort), 2026

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Subsystems/WorldSubsystem.h"
#include "ContextMenuSubsystem.generated.h"

#define UE_API UMGRIGHTCLICKCONTEXTMENUS_API

class UContextMenuSubsystem;
struct FGameplayTag;

DECLARE_DELEGATE_TwoParams(FRegisterMenuActionDelegate, bool bIsAdd, const struct FRCContextMenuActionDescriptor& Descriptor);
DECLARE_DYNAMIC_DELEGATE_OneParam(FExecuteMenuActionDelegate, const FGameplayTag&, ActionTag);

class FRCContextMenu : public TSharedFromThis<FRCContextMenu>
{
public:
	FGameplayTag MenuTag;
	FRegisterMenuActionDelegate Callback;
	int32 Generation = 0;
};

USTRUCT(BlueprintType)
struct FRCContextMenuHandle
{
	GENERATED_BODY()
	
public:
	FRCContextMenuHandle() {  }
	
	UE_API void Unregister();
	bool IsValid() const { return DataPtr.IsValid(); }
	
	bool operator==(const FRCContextMenuHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FRCContextMenuHandle& Other) const { return !operator==(Other); }

	friend uint32 GetTypeHash(const FRCContextMenuHandle& Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}
	
private:
	friend class UContextMenuSubsystem;
	FRCContextMenuHandle(UContextMenuSubsystem* InMenuSubsystem, const TSharedPtr<FRCContextMenu>& InDataPtr)
		: MenuSubsystem(InMenuSubsystem)
		, DataPtr(InDataPtr)
	{
	}
	
	TWeakObjectPtr<UContextMenuSubsystem> MenuSubsystem;
	TSharedPtr<FRCContextMenu> DataPtr;
};


template<>
struct TStructOpsTypeTraits<FRCContextMenuHandle> : public TStructOpsTypeTraitsBase2<FRCContextMenuHandle>
{
	enum
	{
		WithCopy = true,  // This ensures the opaque type is copied correctly in BPs
		WithIdenticalViaEquality = true,
	};
};



class FRCContextMenuAction : public TSharedFromThis<FRCContextMenuAction>
{
public:
	FGameplayTag ActionTag;
	int32 Priority = INDEX_NONE;
	FText DisplayName;
	
	// Menu that this action is intended for
	FRCContextMenuHandle MenuHandle;
	
	FExecuteMenuActionDelegate ExecuteCallback;
};

USTRUCT(BlueprintType)
struct FRCContextMenuActionHandle
{
	GENERATED_BODY()
	
public:
	FRCContextMenuActionHandle() {  }
	
	UE_API void Unregister();
	bool IsValid() const { return DataPtr.IsValid(); }
	
	bool operator==(const FRCContextMenuActionHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FRCContextMenuActionHandle& Other) const { return !operator==(Other); }
	
	friend uint32 GetTypeHash(const FRCContextMenuActionHandle& Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}
	
private:
	friend class UContextMenuSubsystem;
	FRCContextMenuActionHandle(UContextMenuSubsystem* InMenuSubsystem, const TSharedPtr<FRCContextMenuAction>& InDataPtr)
		: MenuSubsystem(InMenuSubsystem)
		, DataPtr(InDataPtr)
	{
	}
	
	TWeakObjectPtr<UContextMenuSubsystem> MenuSubsystem;
	TSharedPtr<FRCContextMenuAction> DataPtr;
};

template<>
struct TStructOpsTypeTraits<FRCContextMenuActionHandle> : public TStructOpsTypeTraitsBase2<FRCContextMenuActionHandle>
{
	enum
	{
		WithCopy = true,  // This ensures the opaque type is copied correctly in BPs
		WithIdenticalViaEquality = true,
	};
};


/** A single context menu action inside a context menu. */
USTRUCT(BlueprintType)
struct FRCContextMenuActionDescriptor
{
	GENERATED_BODY()
	
public:
	FRCContextMenuActionDescriptor() {  }
	
	/** Label to show in the ui. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Action)
	FText DisplayName;
	
	/** Tag used to identify this menu action. 
	 * Will be used to query whether an action with the same tag is already present. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Action)
	FGameplayTag ActionTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Action)
	int32 Priority = INDEX_NONE;
	
	UPROPERTY(BlueprintReadOnly, Category=Action)
	FRCContextMenuActionHandle ActionHandle;
	
	UPROPERTY(BlueprintReadOnly, Category=Action)
	FRCContextMenuHandle MenuHandle;
	
	/** Callback to be invoked when this action gets executed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Action)
	FExecuteMenuActionDelegate ExecuteActionCallback;
};

/**
 * 
 */
UCLASS(MinimalAPI)
class UContextMenuSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	static UE_API UContextMenuSubsystem* Get(const UObject* ContextObject);
	
	//UE_API FGameplayTag GetMenuActionTag(const UUserWidget* Widget) const;
	UE_API void ExecuteMenuAction(const FGameplayTag& ActionTag);
	
	UE_API FRCContextMenuHandle RegisterRCContextMenu(const FGameplayTag& MenuTag, FRegisterMenuActionDelegate RegisterMenuAction);
	UE_API void UnregisterRCContextMenu(const FRCContextMenuHandle& MenuHandle);

	/**
	 * Registers a new right-click context menu action with all context menus
	 * @param Menu Menu to register this action with
	 * @param Action Descriptor of the action to add
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = ContextMenus)
	UE_API FRCContextMenuActionHandle RegisterRCContextMenuAction(FRCContextMenuHandle Menu, FRCContextMenuActionDescriptor Action);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = ContextMenus)
	UE_API void UnregisterRCContextMenuAction(const FRCContextMenuActionHandle& ActionHandle);
	
	UE_API void ExecuteMenuAction(const FRCContextMenuActionHandle& ActionHandle);
	
	UE_API void NotifyMenuOfActions(const FRCContextMenuHandle& Menu);

protected:
	UE_API void NotifyMenuOfActions(const TSharedPtr<FRCContextMenu>& Menu);
	UE_API void NotifyActionOfMenus(bool bIsAdd, const TSharedPtr<FRCContextMenuAction>& Action);
	
protected:
	UE_API FRCContextMenuActionDescriptor CreateActionDescriptor(const TSharedPtr<FRCContextMenuAction>& Action);
	
protected:
	typedef TArray<TSharedPtr<FRCContextMenu>> FMenuList;
	FMenuList MenuList;
	
	typedef TMap<FGameplayTag, TSharedPtr<FRCContextMenuAction>> FActionMap;
	TMap<FRCContextMenuHandle, FActionMap> ActionsPerMenu;
};

#undef UE_API
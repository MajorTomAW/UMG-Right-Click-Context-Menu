// Author: Tom Werner (dc: majort), 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IContextMenuWidgetInterface.generated.h"

struct FRCContextMenuHandle;
struct FGameplayTag;
struct FRCContextMenuActionDescriptor;

#define UE_API UMGRIGHTCLICKCONTEXTMENUS_API

/** Interface for a menu widget that contains multiple menu actions. */
UINTERFACE()
class UContextMenuWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class IContextMenuWidgetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = ContextMenus)
	void OnMenuCreated(const FGameplayTag& MenuTag);
	
	//static UE_API FRCContextMenuHandle GetMenuHandle(UUserWidget* Widget);
	//static UE_API void AddOrRemoveMenuAction(UUserWidget* Widget, bool bIsAdd, const FRCContextMenuActionDescriptor& ActionDescriptor);
	
protected:
	/*UFUNCTION(BlueprintNativeEvent, Category = ContextMenuWidget)
	FRCContextMenuHandle OnGetMenuHandle() const;*/
	
	UFUNCTION(BlueprintNativeEvent, Category = ContextMenus)
	void OnAddOrRemoveMenuAction(bool bIsAdd, const FRCContextMenuActionDescriptor& ActionDescriptor);
	
	/** Registers a new menu action in the context menu. */
	/*UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ContextMenuWidget)
	void RegisterMenuAction(const FRCContextMenuActionDescriptor& ActionDescriptor);*/
};


#undef UE_API
// Author: Tom Werner (dc: majort), 2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IContextMenuActionWidget.generated.h"

struct FRCContextMenuActionHandle;
struct FRCContextMenuHandle;
struct FRCContextMenuActionDescriptor;

#define UE_API UMGRIGHTCLICKCONTEXTMENUS_API

/** Interface to implement in your context menu action widgets to retrieve generic data such as the action descriptor
 * and sending the execute event. */
UINTERFACE()
class UContextMenuActionWidget : public UInterface
{
	GENERATED_BODY()
};

class IContextMenuActionWidget
{
	GENERATED_BODY()
	
public:
	/** Calls either cpp or bp version of the OnMenuActionSet() method. */
	//static UE_API void SetMenuAction(UUserWidget& Widget, const FRCContextMenuActionDescriptor& ActionDescriptor);

	/*/** Executes a menu action. #1#
	UFUNCTION(BlueprintCallable, Category = ContextMenu)
	UE_API virtual void ExecuteMenuAction(const FRCContextMenuActionDescriptor& ActionDescriptor);*/
	
protected:
	/** Called when this menu action is assigned to a new menu action from a context menu. */
	UFUNCTION(BlueprintNativeEvent, Category = ContextMenu)
	UE_API void OnMenuActionSet(const FRCContextMenuActionDescriptor& ActionDescriptor);
};

UCLASS(MinimalAPI)
class UContextMenuActionWidgetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = ContextMenus, meta=(DefaultToSelf=ActionWidget))
	static UE_API void ExecuteMenuAction(TScriptInterface<IContextMenuActionWidget> ActionWidget, const FRCContextMenuActionHandle& Action);
};

#undef UE_API
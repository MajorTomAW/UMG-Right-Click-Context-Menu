// Author: Tom Werner (dc: majort), 2026

#pragma once

#include "CoreMinimal.h"
#include "ContextMenuSubsystem.h"
#include "GameplayTagContainer.h"
#include "IContextMenuWidgetInterface.h"

#include "Blueprint/UserWidget.h"
#include "ContextMenuWidget.generated.h"

#define UE_API UMGRIGHTCLICKCONTEXTMENUS_API

class UDynamicEntryBox;
/**  */
UCLASS(MinimalAPI, Abstract)
class UContextMenuWidget : public UUserWidget, public IContextMenuWidgetInterface
{
	GENERATED_BODY()
	
public:
	//~ Begin IContextMenuWidgetInterface
	/*UE_API virtual void RegisterMenuAction_Implementation(const FRCContextMenuActionDescriptor& ActionDescriptor) override;
	UE_API virtual void OnMenuCreated_Implementation(const FGameplayTag& MenuTag) override;
	*/
	
	virtual void OnAddOrRemoveMenuAction_Implementation(bool bIsAdd, const FRCContextMenuActionDescriptor& ActionDescriptor) override;
	//~ End IContextMenuWidgetInterface
	
protected:
	UE_API virtual void RegisterMenuActionInternal(const FRCContextMenuActionDescriptor& ActionDescriptor);

	
protected:
	/** The widget class to be used for the action entries. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ContextMenu, meta=(DesignerRebuild, MustImplement="/Script/UMGRightClickContextMenus.ContextMenuActionWidget"))
	TSubclassOf<UUserWidget> MenuActionClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ContextMenu, meta=(BindWidget))
	TObjectPtr<UDynamicEntryBox> MenuActionBox;
	
private:
	FGameplayTag MenuTag;
	
	UPROPERTY(Transient)
	TMap<FRCContextMenuActionHandle, TObjectPtr<UUserWidget>> MenuActions;
};

#undef UE_API
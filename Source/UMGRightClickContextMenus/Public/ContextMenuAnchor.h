// Author: Tom Werner (dc: majort), 2026

#pragma once

#include "CoreMinimal.h"
#include "ContextMenuSubsystem.h"
#include "GameplayTagContainer.h"

#include "Components/ContentWidget.h"

#include "ContextMenuAnchor.generated.h"

#define UE_API UMGRIGHTCLICKCONTEXTMENUS_API



/**
 * An anchor for dynamic context menu actions
 */
UCLASS(MinimalAPI)
class UContextMenuAnchor : public UContentWidget
{
	GENERATED_BODY()
	
public:
	/** Tag used to identify this menu anchor among all the other registered anchors. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ContextMenuAnchor)
	FGameplayTag MenuAnchorTag;
	
	/**
	 * The widget class to spawn when the context menu is required.
	 * Creates the widget freshly each time.  
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ContextMenuAnchor, meta=(DesignerRebuild, MustImplement="/Script/UMGRightClickContextMenus.ContextMenuWidgetInterface"))
	TSubclassOf<class UUserWidget> MenuClass;
	
	/** The placement location of the summoned widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter, Getter, BlueprintSetter = "SetPlacement", Category = ContextMenuAnchor)
	TEnumAsByte<EMenuPlacement> Placement;
	
	/** Should the menu anchor attempt to fit the menu inside the window. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = "FitInWindow", Getter = "IsFitInWindow", BlueprintSetter = "FitInWindow", Category = ContextMenuAnchor, meta = (ScriptName = "ShouldFitInWindow"))
	bool bFitInWindow;
	
public:
	UFUNCTION(BlueprintCallable, Category = ContextMenuAnchor)
	UE_API void SetPlacement(EMenuPlacement InPlacement);

	EMenuPlacement GetPlacement() const { return Placement; }
	
	UFUNCTION(BlueprintCallable, Category = ContextMenuAnchor)
	UE_API void FitInWindow(bool bFit);

	bool IsFitInWindow() const { return bFitInWindow; }
	
	UFUNCTION(BlueprintCallable, Category = ContextMenuAnchor)
	UE_API FRCContextMenuHandle GetMenuHandle() const;
	
	/**
	 * Toggles the menus open state.
	 *
	 * @param bFocusOnOpen  Should we focus the popup as soon as it opens?
	 */
	UFUNCTION(BlueprintCallable, Category = ContextMenuAnchor)
	UE_API void ToggleOpen(bool bFocusOnOpen);

	/** Opens the menu if it is not already open */
	UFUNCTION(BlueprintCallable, Category = ContextMenuAnchor)
	UE_API void Open(bool bFocusMenu);

	/** Closes the menu if it is currently open. */
	UFUNCTION(BlueprintCallable, Category = ContextMenuAnchor)
	UE_API void Close();

	/** Returns true if the popup is open; false otherwise. */
	UFUNCTION(BlueprintCallable, Category = ContextMenuAnchor)
	UE_API bool IsOpen() const;
	
protected:
	//~ Begin UPanelWidget Interface
	UE_API virtual void OnSlotAdded(UPanelSlot* Slot) override;
	UE_API virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	//~ End UPanelWidget Interface

	//~ Begin UWidget Interface
	UE_API virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	UE_API virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface
	
	UE_API void ResetRCContextMenu();
	UE_API void RegisterRCContextMenu();
	
	void OnAddOrRemoveMenuAction(bool bIsAdd, const FRCContextMenuActionDescriptor& ActionDescriptor);
	
	UE_API TSharedRef<SWidget> HandleGetMenuContent();
	UE_API void HandleMenuOpenChanged(bool bIsOpen);
	
protected:
	TSharedPtr<SMenuAnchor> MyMenuAnchor;
	TWeakObjectPtr<UUserWidget> MyMenuWidget;
	
	FRCContextMenuHandle MenuHandle;
};

#undef UE_API
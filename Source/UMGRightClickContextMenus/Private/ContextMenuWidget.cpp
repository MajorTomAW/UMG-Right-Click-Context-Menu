// Author: Tom Werner (dc: majort), 2026


#include "ContextMenuWidget.h"

#include "ContextMenuAnchor.h"
#include "ContextMenuLog.h"
#include "IContextMenuActionWidget.h"

#include "Components/DynamicEntryBox.h"

/*void UContextMenuWidget::RegisterMenuAction_Implementation(const FRCContextMenuActionDescriptor& ActionDescriptor)
{
	/*if (MenuActions.Contains(ActionDescriptor.ActionTag))
	{
		UE_LOG(LogContextMenus, Warning, TEXT("MenuAction %s already registered!"), *ActionDescriptor.ActionTag.ToString());
		return;
	}
	
	RegisterMenuActionInternal(ActionDescriptor);#1#
}

void UContextMenuWidget::OnMenuCreated_Implementation(const FGameplayTag& InMenuTag)
{
	MenuTag = InMenuTag;
}*/

void UContextMenuWidget::RegisterMenuActionInternal(const FRCContextMenuActionDescriptor& ActionDescriptor)
{
	/*MenuActions.Add(ActionDescriptor.ActionTag, ActionDescriptor);
	
	UUserWidget* Widget = MenuActionBox->CreateEntry(MenuActionClass);
	IContextMenuActionWidget::SetMenuAction(*Widget, ActionDescriptor);*/
}


void UContextMenuWidget::OnAddOrRemoveMenuAction_Implementation(bool bIsAdd, const FRCContextMenuActionDescriptor& ActionDescriptor)
{
	if (bIsAdd)
	{
		UUserWidget* Widget = MenuActionBox->CreateEntry(MenuActionClass);
		MenuActions.Add(ActionDescriptor.ActionHandle, Widget);
		
		IContextMenuActionWidget::Execute_OnMenuActionSet(Widget, ActionDescriptor);
	}
	else
	{
		if (UUserWidget* Widget = MenuActions.FindRef(ActionDescriptor.ActionHandle))
		{
			MenuActionBox->RemoveEntry(Widget);
			MenuActions.Remove(ActionDescriptor.ActionHandle);
		}
	}
}

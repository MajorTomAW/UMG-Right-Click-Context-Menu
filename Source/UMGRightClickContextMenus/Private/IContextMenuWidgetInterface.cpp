// Author: Tom Werner (dc: majort), 2026


#include "IContextMenuWidgetInterface.h"

#include "ContextMenuSubsystem.h"

#include "Blueprint/UserWidget.h"

/*FRCContextMenuHandle IContextMenuWidgetInterface::GetMenuHandle(UUserWidget* Widget)
{
	if (!IsValid(Widget))
	{
		return FRCContextMenuHandle();
	}
	
	if (ThisClass* NativeImpl = Cast<ThisClass>(Widget))
	{
		return NativeImpl->OnGetMenuHandle();
	}
	else if (Widget->Implements<UContextMenuWidgetInterface>())
	{
		return Execute_OnGetMenuHandle(Widget);
	}
	
	return FRCContextMenuHandle();
}*/

/*void IContextMenuWidgetInterface::AddOrRemoveMenuAction(
	UUserWidget* Widget, 
	bool bIsAdd, 
	const FRCContextMenuActionDescriptor& ActionDescriptor)
{
	if (!IsValid(Widget))
	{
		return;
	}
	
	if (IContextMenuWidgetInterface* NativeImpl = Cast<IContextMenuWidgetInterface>(Widget))
	{
		NativeImpl->OnAddOrRemoveMenuAction(bIsAdd, ActionDescriptor);
	}
	else if (Widget->Implements<UContextMenuWidgetInterface>())
	{
		Execute_OnAddOrRemoveMenuAction(Widget, bIsAdd, ActionDescriptor);
	}
}*/

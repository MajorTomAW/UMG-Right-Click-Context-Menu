// Author: Tom Werner (dc: majort), 2026

#include "IContextMenuActionWidget.h"

#include "ContextMenuSubsystem.h"

#include "Blueprint/UserWidget.h"

/*void IContextMenuActionWidget::SetMenuAction(UUserWidget& Widget, const FRCContextMenuActionDescriptor& ActionDescriptor)
{
	if (ThisClass* NativeImpl = Cast<ThisClass>(&Widget))
	{
		NativeImpl->OnMenuActionSet(ActionDescriptor);
	}
	else if (Widget.Implements<UContextMenuActionWidget>())
	{
		Execute_OnMenuActionSet(&Widget, ActionDescriptor);
	}
}*/

/*void IContextMenuActionWidget::ExecuteMenuAction(const FRCContextMenuActionDescriptor& ActionDescriptor)
{
	auto* Widget = Cast<UUserWidget>(this);
	check(Widget)
	
	//UContextMenuSubsystem::Get(Widget->GetOwningLocalPlayer())->ExecuteMenuAction()
}*/
void UContextMenuActionWidgetLibrary::ExecuteMenuAction(
	TScriptInterface<IContextMenuActionWidget> ActionWidget, 
	const FRCContextMenuActionHandle& Action)
{
	UUserWidget* EntryWidget = Cast<UUserWidget>(ActionWidget.GetObject());
	if (!IsValid(EntryWidget))
	{
		return;
	}
	
	UContextMenuSubsystem::Get(EntryWidget)->ExecuteMenuAction(Action);
}

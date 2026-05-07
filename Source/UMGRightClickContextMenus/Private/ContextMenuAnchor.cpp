// Author: Tom Werner (dc: majort), 2026


#include "ContextMenuAnchor.h"

#include "ContextMenuLog.h"
#include "IContextMenuWidgetInterface.h"

#define LOCTEXT_NAMESPACE "ContextMenuAnchor"

void UContextMenuAnchor::SetPlacement(EMenuPlacement InPlacement)
{
	Placement = InPlacement;
	if (MyMenuAnchor.IsValid())
	{
		MyMenuAnchor->SetMenuPlacement(Placement);
	}
}

void UContextMenuAnchor::FitInWindow(bool bFit)
{
	bFitInWindow = bFit;
	if (MyMenuAnchor.IsValid())
	{
		MyMenuAnchor->SetFitInWindow(bFitInWindow);
	}
}

FRCContextMenuHandle UContextMenuAnchor::GetMenuHandle() const
{
	return MenuHandle;
}

void UContextMenuAnchor::ToggleOpen(bool bFocusOnOpen)
{
	if (MyMenuAnchor.IsValid())
	{
		MyMenuAnchor->SetIsOpen(!MyMenuAnchor->IsOpen(), bFocusOnOpen);
	}
}

void UContextMenuAnchor::Open(bool bFocusMenu)
{
	if (MyMenuAnchor.IsValid() && !MyMenuAnchor->IsOpen())
	{
		const int32 UserIdx = GetOwningLocalPlayer() ? GetOwningLocalPlayer()->GetLocalPlayerIndex() : 0;
		MyMenuAnchor->SetIsOpen(true, bFocusMenu, UserIdx);
	}
}

void UContextMenuAnchor::Close()
{
	if (MyMenuAnchor.IsValid() && MyMenuAnchor->IsOpen())
	{
		const int32 UserIdx = GetOwningLocalPlayer() ? GetOwningLocalPlayer()->GetLocalPlayerIndex() : 0;
		MyMenuAnchor->SetIsOpen(false, false, UserIdx);
	}
}

bool UContextMenuAnchor::IsOpen() const
{
	if (MyMenuAnchor.IsValid())
	{
		return MyMenuAnchor->IsOpen();
	}
	
	return false;
}

void UContextMenuAnchor::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if ( MyMenuAnchor.IsValid() )
	{
		MyMenuAnchor->SetContent(InSlot->Content ? InSlot->Content->TakeWidget() : SNullWidget::NullWidget);
	}
}

void UContextMenuAnchor::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyMenuAnchor.IsValid() )
	{
		MyMenuAnchor->SetContent(SNullWidget::NullWidget);
	}
}

void UContextMenuAnchor::ReleaseSlateResources(bool bReleaseChildren)
{
	ResetRCContextMenu();
	
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UContextMenuAnchor::ResetRCContextMenu()
{
	MenuHandle.Unregister();
}

void UContextMenuAnchor::RegisterRCContextMenu()
{
	if (UContextMenuSubsystem* Sub = UContextMenuSubsystem::Get(GetWorld()))
	{
		MenuHandle = Sub->RegisterRCContextMenu(MenuAnchorTag,
			FRegisterMenuActionDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveMenuAction));
	}
}

void UContextMenuAnchor::OnAddOrRemoveMenuAction(bool bIsAdd, const FRCContextMenuActionDescriptor& ActionDescriptor)
{
	// Context Menu hasnt been opened yet
	if (!MyMenuWidget.IsValid())
	{
		return;
	}
	
	IContextMenuWidgetInterface::Execute_OnAddOrRemoveMenuAction(MyMenuWidget.Get(), bIsAdd, ActionDescriptor);
}


TSharedRef<SWidget> UContextMenuAnchor::RebuildWidget()
{
	if (!IsDesignTime() && MenuAnchorTag.IsValid())
	{
		ResetRCContextMenu();
		RegisterRCContextMenu();
		
		UE_LOG(LogContextMenus, Warning, TEXT("REbuilding menu anchor"))

PRAGMA_DISABLE_DEPRECATION_WARNINGS
		MyMenuAnchor = SNew(SMenuAnchor)
			.Placement(Placement)
			.FitInWindow(bFitInWindow)
			.OnGetMenuContent(BIND_UOBJECT_DELEGATE(FOnGetContent, HandleGetMenuContent))
			.OnMenuOpenChanged(BIND_UOBJECT_DELEGATE(FOnIsOpenChanged, HandleMenuOpenChanged))
			.ShouldDeferPaintingAfterWindowContent(true)
			.UseApplicationMenuStack(true)
			.ShowMenuBackground(false);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
		
		if ( GetChildrenCount() > 0 )
		{
			MyMenuAnchor->SetContent(
				GetContentSlot()->Content 
				? GetContentSlot()->Content->TakeWidget() 
				: SNullWidget::NullWidget);
		}
	
		return MyMenuAnchor.ToSharedRef();
	}
	
	auto GetMenuAnchorText = [this] ()
	{
		return FText::Format(LOCTEXT("DesignTime_ContextMenuAnchorLabel", "Context Menu: {0}"), FText::FromName(MenuAnchorTag.GetTagName()));
	};
	
	TSharedRef<SOverlay> MessageBox = SNew(SOverlay);

	MessageBox->AddSlot()
		.Padding(2.0f)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Center)
			.Text_Lambda(GetMenuAnchorText)
		];

	return MessageBox;
}


TSharedRef<SWidget> UContextMenuAnchor::HandleGetMenuContent()
{
	TSharedPtr<SWidget> SlateMenuWidget;
	
	if (IsValid(MenuClass) && !MenuClass->HasAnyClassFlags(CLASS_Abstract))
	{
		if (UWorld* World = GetWorld())
		{
			if (UUserWidget* MenuWidget = CreateWidget(World, MenuClass))
			{
				MyMenuWidget = MenuWidget;
				SlateMenuWidget = MenuWidget->TakeWidget();
				
				// We want to get all menu actions
				UContextMenuSubsystem::Get(World)->NotifyMenuOfActions(GetMenuHandle());
			}
		}
	}
	
	return SlateMenuWidget.IsValid() ? SlateMenuWidget.ToSharedRef() : SNullWidget::NullWidget;
}

void UContextMenuAnchor::HandleMenuOpenChanged(bool bIsOpen)
{
}



#undef LOCTEXT_NAMESPACE

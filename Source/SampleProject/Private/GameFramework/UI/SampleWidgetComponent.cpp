// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/UI/SampleWidgetComponent.h"
#include "GameFramework/UI/SampleUserWidget.h"
#include "Components/TextBlock.h"


USampleWidgetComponent::USampleWidgetComponent()
{
	// Set common defaults when using widgets on Actors
	SetDrawAtDesiredSize(true);
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USampleWidgetComponent::InitWidget()
{
	// TODO: Comment
	Super::InitWidget();

	if (GetUserWidgetObject() != nullptr)
	{
#if !UE_BUILD_SHIPPING
		if (!GetUserWidgetObject()->IsA(USampleUserWidget::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetClass for %s does not derive from UW3UserWidget"), *GetNameSafe(GetOwner()));
		}
#endif
		USampleUserWidget* WidgetInst = Cast<USampleUserWidget>(GetUserWidgetObject());
		if (WidgetInst != nullptr)
		{
			WidgetInst->SetOwningActor(GetOwner());
		}
	}
}

void USampleWidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if !UE_SERVER
	if (!IsRunningDedicatedServer())
	{
		DisplayTextObject();
	}
#endif // !UE_SERVER	
}

void USampleWidgetComponent::DisplayTextObject()
{
	if (GetUserWidgetObject() != nullptr)
	{
		USampleUserWidget* WidgetInst = Cast<USampleUserWidget>(GetUserWidgetObject());
		if (WidgetInst != nullptr && WidgetInst->DisplayText != nullptr)
		{			
			WidgetInst->DisplayText->SetText(FText::FromString(ActionType + ObjectName));
		}
	}
}
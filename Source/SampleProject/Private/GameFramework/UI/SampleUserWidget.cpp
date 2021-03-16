// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/UI/SampleUserWidget.h"
#include "GameFramework/Player/SampleCharacter.h"
#include "GameFramework/Player/SamplePlayerControllerBase.h"

void USampleUserWidget::SetOwningActor(AActor* NewOwner)
{
	if (OwningActor == NewOwner)
	{
		// Skip repeated calls
		return;
	}

	OwningActor = NewOwner;

	OnOwningActorChanged.Broadcast(NewOwner);
}

void USampleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<ASamplePlayerControllerBase>(GetOwningPlayer());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Construct: Player: %s Failed to Cast!!"), *GetNameSafe(PlayerController));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Construct: Player: %s CASTED !!"), *GetNameSafe(PlayerController));
	}	
}

bool USampleUserWidget::Initialize()
{
	bool Succes = Super::Initialize();

	if (!Succes)
	{
		return false;
	}

	PlayerController = Cast<ASamplePlayerControllerBase>(GetOwningPlayer());
	Player = Cast<ASampleCharacter>(GetOwningPlayerPawn());

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget Init: Failed %s to Cast!!"), *GetNameSafe(PlayerController));
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget Init: %S CASTED !!"), *GetNameSafe(PlayerController));
	}

	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget Init: Player: %s Failed to Cast!!"), *GetNameSafe(Player));
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget Init: Player: %s CASTED !!"), *GetNameSafe(Player));
	}

	return true;
}

void USampleUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	
}
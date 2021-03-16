// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/UI/SampleHUD.h"
#include "GameFramework/Player/SampleCharacter.h"
#include "GameFramework/Player/SamplePlayerControllerBase.h"
#include "GameFramework/UI/SampleUserWidget.h"
#include "UObject/ConstructorHelpers.h"

ASampleHUD::ASampleHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	if (CrosshairText != nullptr)
	{
		CenterDotIcon = UCanvas::MakeIcon(CrosshairText);
	}
	else
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> HUDCenterDotObj(TEXT("/Game/UI/HUD/T_CenterDot_M.T_CenterDot_M"));
		CenterDotIcon = UCanvas::MakeIcon(HUDCenterDotObj.Object);
	}

	//bPlayerPickedUp = false;
}

void ASampleHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCenterDot();	
}

void ASampleHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateInventorySmallWidget();
	CreateInventoryWidget();
}

void ASampleHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASampleHUD::DrawCenterDot()
{
	float CenterX = Canvas->ClipX / 2;
	float CenterY = Canvas->ClipY / 2;
	float CenterDotScale = 0.07f;

	ASampleCharacter* Pawn = Cast<ASampleCharacter>(GetOwningPawn());
	if (Pawn != nullptr)
	{
		Canvas->SetDrawColor(255, 255, 255, 255);
		Canvas->DrawIcon(CenterDotIcon, 
			CenterX - CenterDotIcon.UL *CenterDotScale / 2.0f, 
			CenterY - CenterDotIcon.VL *CenterDotScale / 2.0f, CenterDotScale);		
	}
}

void ASampleHUD::CreateInventorySmallWidget()
{
	// TODO: else comment
	if (InventorySmallWidgetClass != nullptr)
	{
		InventorySmallWidget = CreateWidget<USampleUserWidget>(GetOwningPlayerController(), InventorySmallWidgetClass);
		InventorySmallWidget->AddToViewport();
	}
}

void ASampleHUD::CreateInventoryWidget()
{
	// TODO: else comment
	if (InventoryWidgetClass != nullptr)
	{
		InventoryWidget = CreateWidget<USampleUserWidget>(GetOwningPlayerController(), InventoryWidgetClass);
	}
}



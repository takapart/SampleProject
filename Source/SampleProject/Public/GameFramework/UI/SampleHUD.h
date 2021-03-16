// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "GameFramework/Player/SamplePlayerControllerBase.h"
#include "Engine/Canvas.h"
#include "SampleHUD.generated.h"

/**
 * Holder for widgets
 */
UCLASS()
class SAMPLEPROJECT_API ASampleHUD : public AHUD
{
	GENERATED_BODY()

	// Variables

	// Class for smallest version of player's inventory 
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<class USampleUserWidget> InventorySmallWidgetClass;

	// Class for an extended version of player's inventory 
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<class USampleUserWidget> InventoryWidgetClass;

public: // Variables

	// Smallest version of player's inventory 
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	class USampleUserWidget* InventorySmallWidget;

	// An extended version of player's inventory
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	class USampleUserWidget* InventoryWidget;


	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	UTexture2D* CrosshairText;

	FCanvasIcon CenterDotIcon;

public: // Methods

	ASampleHUD(const FObjectInitializer& ObjectInitializer);

	virtual void DrawHUD() override;
	virtual void BeginPlay() override;	

	virtual void Tick(float DeltaSeconds) override;

	void DrawCenterDot();
	
	void CreateInventorySmallWidget();
	void CreateInventoryWidget();
};



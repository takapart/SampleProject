// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "SampleWidgetComponent.generated.h"

/**
 * Default widget component class, which are storing and holding variables and functions to displays on screen, when player is looking at item
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class SAMPLEPROJECT_API USampleWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public: // Variables
	
	// Object name of interactable item to display into on screen
	FString ObjectName;
	
	// Displays hint about action, which we can take  
	FString ActionType;

public: // Methods

	// Init default values on start widget
	virtual void InitWidget() override;

	// Refresh widget's properties every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	USampleWidgetComponent();

	// Function to display and connect 
	void DisplayTextObject();
};

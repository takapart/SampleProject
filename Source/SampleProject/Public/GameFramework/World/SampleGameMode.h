// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/GameMode.h"
#include "SampleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ASampleGameMode : public AGameMode
{
	GENERATED_BODY()
	
public: // Methods

	ASampleGameMode();

	// Get Data about storable items to inventory
	class UDataTable* GetItemDB() const { return ItemDB; }

protected: // Variables

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDB;
};

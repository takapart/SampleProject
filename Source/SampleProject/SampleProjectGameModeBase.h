// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "SampleProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ASampleProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASampleProjectGameModeBase();

	// Get Data about Storable Items 
	class UDataTable* GetItemDB() const { return ItemDB; }

protected:


	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDB;
};

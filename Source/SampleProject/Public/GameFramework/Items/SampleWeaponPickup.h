// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Items/SamplePickupObject.h"
#include "SampleWeaponPickup.generated.h"

/**
 * 
 */

UCLASS(Abstract)
class SAMPLEPROJECT_API ASampleWeaponPickup : public ASamplePickupObject
{
	GENERATED_BODY()

	ASampleWeaponPickup(const FObjectInitializer& ObjectInitializer);

public: // Variables

	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
	TSubclassOf<class ASampleWeapon> WeaponClass;

public: // Methods

	virtual void OnUsed(APawn* InstigatorPawn) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Items/SampleInteractableObject.h"
#include "SamplePickupObject.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SAMPLEPROJECT_API ASamplePickupObject : public ASampleInteractableObject
{
	GENERATED_BODY()

	// Variables

	// Played sound at pickup
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* PickupSound;

private: // Methods

	void BeginPlay() override;

	UFUNCTION()
	void OnRep_IsActive();

protected: // Variables

	// ItemID to recognize from DataDB 
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	FName ItemID;

	// Describes when item was picked up by player, replicated to other players
	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;

protected: // Methods

	ASamplePickupObject(const FObjectInitializer& ObjectInitializer);

	virtual void RespawnPickup();

	virtual void OnPickedUp();

	virtual void OnRespawned();

public: // Variables

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	bool bStartActive;

	// Respawn taken item after described time by RespawnDelay and RespawnDelayRange
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	bool bAllowRespawn;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnDelayRange;

public: // Methods

	virtual void OnUsed(APawn* InstigatorPawn) override;
};

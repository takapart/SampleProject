// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SampleEffectActor.generated.h"

/**
 * Spawning actor class for replicating particles and audio effects
 */

UCLASS()
class SAMPLEPROJECT_API ASampleEffectActor : public AActor
{
	GENERATED_BODY()
	
public: // Variables

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystemComponent* EffectsComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UAudioComponent* AudioComp;

public: // Methods

	// Sets default values for this actor's properties
	ASampleEffectActor(const FObjectInitializer& ObjectInitializer);

protected: // Methods

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Items/SampleInteractableObject.h"
#include "SampleLightingObject.generated.h"


/**
 * TODO: Comment this class
 */
UCLASS()
class SAMPLEPROJECT_API ASampleLightingObject : public ASampleInteractableObject
{
	GENERATED_BODY()

	// Variables

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UPointLightComponent* PointLightComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float LightIntensity;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundCue* SwitchingFXSound;

private: // Methods

	void ToggleLight(bool Enabled);

	void UpdateWidgetText(bool Enabled);

	UFUNCTION()
	void OnRep_IsActive();

public: // Variables

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;

public: // Methods

	// Sets default values for this actor's properties
	ASampleLightingObject(const FObjectInitializer& ObjectInitializer);

	/* Called when player interacts with object */
	virtual void OnUsed(APawn* InstigatorPawn) override;

protected: // Methods

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};

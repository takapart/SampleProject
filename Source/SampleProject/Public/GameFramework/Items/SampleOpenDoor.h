// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Items/SampleInteractableObject.h"
#include "SampleOpenDoor.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ASampleOpenDoor : public ASampleInteractableObject
{
	GENERATED_BODY()

	// Variables

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	class UMaterialInterface* DestroyedMaterial;

	UPROPERTY(Transient)
	FVector OpenVector = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* DestroyFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundCue* DestroyFXSound;

	bool bOpeningDoor;
	bool bClosingDoor;
	bool bIsDoorClosed;

	float DotProd;
	float MaxDegree;
	float AddedRotation;
	float PosNeg;
	float CurrentDoorRotation;

private: // Methods

	UFUNCTION()
	void OnRep_IsActive();

	void ToggleDoor(bool Status);

	UFUNCTION()
	void OpenDoor(float dt);

	UFUNCTION()
	void CloseDoor(float dt);

	void UpdateWidgetText(bool Status);

	void SetDestroyedWidgetText();

public: // Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USampleHealthComponent* HealthComp;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;	

	UPROPERTY(ReplicatedUsing = OnRep_Destroyed)
	bool bDestroyed;

public: // Methods

	ASampleOpenDoor(const FObjectInitializer& ObjectInitializer);

	/* Called when player interacts with object */
	virtual void OnUsed(APawn* InstigatorPawn) override;

	UFUNCTION()
	void OnRep_Destroyed();

	UFUNCTION()
	void OnHealthChanged(USampleHealthComponent* ObjectHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected: // Methods

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};

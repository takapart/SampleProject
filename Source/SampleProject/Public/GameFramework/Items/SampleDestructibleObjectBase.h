// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SampleDestructibleObjectBase.generated.h"

UCLASS()
class SAMPLEPROJECT_API ASampleDestructibleObjectBase : public AActor
{
	GENERATED_BODY()
	
public: // Methods

	// Sets default values for this actor's properties
	ASampleDestructibleObjectBase(const FObjectInitializer& ObjectInitializer);

protected: // Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* DestroyedMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	class UMaterialInterface* DestroyedMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USampleHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* DestroyFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class USoundCue* DestroyFXSound;

	UPROPERTY(ReplicatedUsing = OnRep_Destroyed)
	bool bDestroyed;

protected: // Methods

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_Destroyed();

	UFUNCTION()
	void OnHealthChanged(USampleHealthComponent* ObjectHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public: // Methods

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};


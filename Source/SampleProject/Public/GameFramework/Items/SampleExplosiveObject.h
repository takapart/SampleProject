// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Items/SampleDestructibleObjectBase.h"
#include "SampleExplosiveObject.generated.h"

/**
 * Explosive actor class
 */
UCLASS()
class SAMPLEPROJECT_API ASampleExplosiveObject : public AActor
{
	GENERATED_BODY()

public: // Methods

	// Sets default values for this actor's properties
	ASampleExplosiveObject(const FObjectInitializer& ObjectInitializer);

protected: // Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// Changing material after explosion
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UMaterialInterface* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ExplosionFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	class USoundCue* ExplosionFXSound;

	class ASampleEffectActor* EffectActorClassToSpawn;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<ASampleEffectActor> EffectActorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USampleHealthComponent* HealthComp;

	UPROPERTY(ReplicatedUsing = OnRep_Exploded)
	bool bExploded;

	/* For RadialDamage */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UDamageType> BarrelDamageType;

	/* For RadialDamage */
	UPROPERTY(EditInstanceOnly, Category = "RadialDamage Effects")
	float BaseDamage;

	/* For RadialDamage */
	UPROPERTY(EditInstanceOnly, Category = "RadialDamage Effects")
	float DamageRadius;

	/* For RadialForceComponent */
	UPROPERTY(EditInstanceOnly, Category = "RadialForceComp Effects")
	float RadialForceStrength;

	/* For RadialForceComponent */
	UPROPERTY(EditInstanceOnly, Category = "RadialForceComp Effects")
	float RadialRadius;

	/* For RadialForceComponent */
	UPROPERTY(EditInstanceOnly, Category = "RadialForceComp Effects")
	float RadialImpulseStrength;

	// Strength for Launching Barrel up after explosion
	UPROPERTY(EditInstanceOnly, Category = "Launch Barrel Up")
	float LaunchStrength;

protected: // Methods

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Replication to the client player
	UFUNCTION()
	void OnRep_Exploded();

	UFUNCTION()
	void OnHealthChanged(USampleHealthComponent* ObjectHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public: // Methods

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};

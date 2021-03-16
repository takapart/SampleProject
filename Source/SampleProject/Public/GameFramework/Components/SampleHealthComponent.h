// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SampleHealthComponent.generated.h"

/**
 * Health Component is using for delegate changes on health value when DamageCauser (e.x. Player) is doing damage to his owner.
 * Every damage is broadcasted with another function **HandleTakeAnyDamage**
 * Connected with replicated bool bIsDead is sending to other player information about state of Owner's HealthComponent  
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, USampleHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup = (Coop), meta = (BlueprintSpawnableComponent))
class SAMPLEPROJECT_API USampleHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public: // Methods

	// Sets default values for this component's properties
	USampleHealthComponent();

protected: // Variables

	bool bIsDead;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultHealth;

protected: // Methods

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_Health(float OldHealth);

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public: // Variables

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

public: // Methods

	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float SetHealth(float HealthToSet) { return Health = HealthToSet; }
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SampleWeapon.generated.h"

class ASampleCharacter;

// FHitScanTrace struct for replicating impact points weapon effects 

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

/**
 * Base class for weapon 
 */

UCLASS()
class SAMPLEPROJECT_API ASampleWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASampleWeapon(const FObjectInitializer& ObjectInitializer);

protected: // Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class USoundCue* ShotFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireIronsightsMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float WeaponRange;

	/* Bullet Spread in Degrees */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpread;

	// FHitScanTrace struct for replicating impact points weapon effects 

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;


protected: // Methods

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayFireEffects(FVector TraceEnd);

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	virtual void Fire();

	void CheckShotHit(const FHitResult& InHitResult, const ASampleCharacter* InOwnerCharacter, const FVector& HitFromDirection, EPhysicalSurface OutPhysicalSurface, FVector& InTracerEndPoint);

	FHitResult LineSingleWeaponTrace(const ASampleCharacter* InOwnerCharacter, const FVector& TraceFrom, const FVector& TraceTo) const;

	FVector GetShotDirection(const ASampleCharacter* InOwnerCharacter, FVector& OutLocation, FRotator& OutRotation) const;

	FVector GetMuzzleLocation() const;

	FVector GetMuzzleDirection() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	UFUNCTION()
	void OnRep_HitScanTrace();

public: // Methods

	void StartFire();

	void StopFire();
};

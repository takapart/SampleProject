// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SampleAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API USampleAnimInstance : public UAnimInstance
{
	friend class ASampleCharacter;

	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	ASampleCharacter* CharacterOwner;

	UPROPERTY(BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	class UCharacterMovementComponent* CharacterOwnerMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	FVector CharacterLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	FRotator CharacterRotation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	FVector JumpingVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float AimOffsetYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float AimOffsetYawInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float AimOffsetPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	float AimOffsetPitchInterpSpeed;

public:
	USampleAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void NativeInitializeAnimation();

	virtual void NativeUpdateAnimation(float DeltaSeconds);
};

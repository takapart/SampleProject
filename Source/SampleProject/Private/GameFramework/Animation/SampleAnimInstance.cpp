// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Animation/SampleAnimInstance.h"
#include "GameFramework/Player/SampleCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


USampleAnimInstance::USampleAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsAiming = false;

	AimOffsetPitchInterpSpeed = 15.0f;
	AimOffsetYawInterpSpeed = 15.0f;
}

void USampleAnimInstance::NativeInitializeAnimation()
{
	CharacterOwner = Cast<ASampleCharacter>(TryGetPawnOwner());
	if (IsValid(CharacterOwner))
	{
		CharacterLocation = CharacterOwner->GetActorLocation();
		CharacterRotation = CharacterOwner->GetActorRotation();

		CharacterOwnerMovement = CharacterOwner->GetCharacterMovement();
	}
}

void USampleAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(CharacterOwner) && IsValid(CharacterOwnerMovement) && DeltaSeconds > 0.0f)
	{
		CharacterLocation = CharacterOwner->GetActorLocation();
		CharacterRotation = CharacterOwner->GetActorRotation();

		Velocity = CharacterOwner->GetVelocity();
		Speed = Velocity.Size2D();
		Direction = CalculateDirection(Velocity, CharacterRotation);
		bIsFalling = CharacterOwnerMovement->IsFalling();
		bIsJumping = bIsFalling && Velocity.Z > 0.0f;
		bIsCrouched = CharacterOwnerMovement->IsCrouching();
		//bIsAiming = CharacterOwner->IsAiming();

		AimOffsetPitch = UKismetMathLibrary::FInterpTo(AimOffsetPitch, UKismetMathLibrary::NormalizedDeltaRotator(CharacterOwner->GetBaseAimRotation(), CharacterRotation).Pitch, DeltaSeconds, AimOffsetPitchInterpSpeed);
		AimOffsetYaw = UKismetMathLibrary::FInterpTo(AimOffsetYaw, UKismetMathLibrary::NormalizedDeltaRotator(CharacterOwner->GetBaseAimRotation(), CharacterRotation).Yaw, DeltaSeconds, AimOffsetYawInterpSpeed);

		AimOffsetPitch = FMath::Clamp<float>(AimOffsetPitch, -90.0f, 90.0f);
		AimOffsetYaw = FMath::Clamp<float>(AimOffsetYaw, -90.0f, 90.0f);
	}
}

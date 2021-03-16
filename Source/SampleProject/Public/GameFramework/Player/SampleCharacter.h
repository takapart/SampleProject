// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Player/SampleBaseCharacter.h"
#include "SampleCharacter.generated.h"

/**
 * Inherited version of player character from player base class 
 */
UCLASS()
class SAMPLEPROJECT_API ASampleCharacter : public ASampleBaseCharacter
{
	GENERATED_BODY()

	// Variables

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraSpringComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AimedWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float NonAimedWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AimedCrouchWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float NonAimedCrouchWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float AimedAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float NonAimedAcceleration;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName MoveForwardInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName MoveRightInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName TurnInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName TurnRateInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName LookUpInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName LookUpRateInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName UseInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName OpenInventoryInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName JumpInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName FireInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName CrouchInputName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), AdvancedDisplay)
	FName AimInputName;

private: // Methods

	ASampleCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected: // Variables

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ASampleWeapon> StarterWeaponClass;

	UPROPERTY(Replicated)
	ASampleWeapon* CurrentWeapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName WeaponAttachSocketName;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float MaxUseDistance;

	bool bHasNewFocus;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsAiming)
	bool bIsAiming;

	UPROPERTY(Transient, Replicated)
	uint8 RemoteViewYaw;

public: // Variables

	class ASampleInteractableObject* FocusedInteractableObject;

public: // Methods

	virtual void MoveForward(float Value);

	virtual void MoveRight(float Value);

	virtual void TurnAtRate(float Rate);

	virtual void LookUpAtRate(float Rate);

	virtual void Use();

	virtual void OpenInventory();

	void CrouchInput();

	void AimInput();

	// Overrided function from base class for adding replication for Yaw rotation
	virtual FRotator GetBaseAimRotation() const override;

	virtual void PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker) override;

	void SetRemoteViewYaw(float NewRemoteViewYaw);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAim();

	UFUNCTION()
	void OnRep_IsAiming();

	void StartFire();

	void StopFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUse();

	void ServerUse_Implementation();

	bool ServerUse_Validate();

	class ASampleInteractableObject* GetInteractableInView();	

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComp; }

	FORCEINLINE bool IsAiming() const { return bIsAiming; }
};




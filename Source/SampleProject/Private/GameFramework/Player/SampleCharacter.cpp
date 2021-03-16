// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Player/SampleCharacter.h"
#include "GameFramework/Items/SampleWeapon.h"
#include "GameFramework/Player/SamplePlayerControllerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Items/SampleInteractableObject.h"
#include "GameFramework/Animation/SampleAnimInstance.h"
#include "SampleProject/SampleProject.h"

#include "DrawDebugHelpers.h"

ASampleCharacter::ASampleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	CameraSpringComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));
	if (CameraSpringComp != nullptr)
	{
		CameraSpringComp->SocketOffset = FVector(0, 35, 0);
		CameraSpringComp->TargetOffset = FVector(0, 0, 55);
		CameraSpringComp->bUsePawnControlRotation = true;
		CameraSpringComp->TargetArmLength = 300.0f;
		CameraSpringComp->SetupAttachment(RootComponent);
	}

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (CameraComp != nullptr)
	{
		CameraComp->bUsePawnControlRotation = false;
		CameraComp->SetupAttachment(CameraSpringComp, USpringArmComponent::SocketName);
	}

	// Input Settings
	MoveForwardInputName = TEXT("MoveForward");
	MoveRightInputName = TEXT("MoveRight");
	TurnInputName = TEXT("Turn");
	TurnRateInputName = TEXT("TurnRate");
	LookUpInputName = TEXT("LookUp");
	LookUpRateInputName = TEXT("LookUpRate");
	UseInputName = TEXT("Use");
	OpenInventoryInputName = TEXT("OpenInventory");
	JumpInputName = TEXT("Jump");
	FireInputName = TEXT("Fire");
	CrouchInputName = TEXT("Crouch");
	AimInputName = TEXT("Aim");

	MaxUseDistance = 500;
	bHasNewFocus = true;	

	bIsAiming = false;

	WeaponAttachSocketName = TEXT("WeaponSocket");
}

void ASampleCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		if (StarterWeaponClass != nullptr)
		{
			FActorSpawnParameters WeaponSpawnParams;
			WeaponSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentWeapon = GetWorld()->SpawnActor<ASampleWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, WeaponSpawnParams);

			if (CurrentWeapon != nullptr)
			{
				CurrentWeapon->SetOwner(this);
				CurrentWeapon->SetInstigator(this);
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
			}
		}

		UCharacterMovementComponent* CharMovement = GetCharacterMovement();
		if (CharMovement != nullptr)
		{
			NonAimedWalkSpeed = CharMovement->MaxWalkSpeed;
			NonAimedCrouchWalkSpeed = CharMovement->MaxWalkSpeedCrouched;
			NonAimedAcceleration = CharMovement->MaxAcceleration;
		}
	}
}

void ASampleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller != nullptr && Controller->IsLocalController())
	{
		ASampleInteractableObject* Interactable = GetInteractableInView();
		
		// TODO: Comment
		if (FocusedInteractableObject != Interactable)
		{
			if (FocusedInteractableObject != nullptr)
			{
				FocusedInteractableObject->OnEndFocus();
			}

			bHasNewFocus = true;
		}

		// TODO: Comment
		FocusedInteractableObject = Interactable;

		// TODO: Comment
		if (Interactable != nullptr)
		{			
			if (bHasNewFocus)
			{
				Interactable->OnBeginFocus();
				bHasNewFocus = false;
			}
		}
	}	
}

// Called to bind functionality to input
void ASampleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis(MoveForwardInputName, this, &ASampleCharacter::MoveForward);
	PlayerInputComponent->BindAxis(MoveRightInputName, this, &ASampleCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TurnInputName, this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TurnRateInputName, this, &ASampleCharacter::TurnAtRate);

	PlayerInputComponent->BindAxis(LookUpInputName, this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(LookUpRateInputName, this, &ASampleCharacter::LookUpAtRate);

	// Setup gameplay key bindings
	PlayerInputComponent->BindAction(UseInputName, IE_Pressed, this, &ASampleCharacter::Use);
	PlayerInputComponent->BindAction(OpenInventoryInputName, IE_Pressed, this, &ASampleCharacter::OpenInventory);
	PlayerInputComponent->BindAction(JumpInputName, IE_Pressed, this, &ASampleCharacter::Jump);
	PlayerInputComponent->BindAction(JumpInputName, IE_Released, this, &ASampleCharacter::StopJumping);
	PlayerInputComponent->BindAction(FireInputName, IE_Pressed, this, &ASampleCharacter::StartFire);
	PlayerInputComponent->BindAction(FireInputName, IE_Released, this, &ASampleCharacter::StopFire);
	PlayerInputComponent->BindAction(CrouchInputName, IE_Pressed, this, &ASampleCharacter::CrouchInput);
	PlayerInputComponent->BindAction(AimInputName, IE_Pressed, this, &ASampleCharacter::AimInput);
	PlayerInputComponent->BindAction(AimInputName, IE_Released, this, &ASampleCharacter::AimInput);
}

void ASampleCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const bool bLimitedRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		const FRotator Rotation = bLimitedRotation ? GetActorRotation() : Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void ASampleCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}


void ASampleCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(BaseTurnRate > 0.0f ? Rate* BaseTurnRate * GetWorld()->GetDeltaSeconds() : Rate);
}


void ASampleCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(BaseLookUpRate > 0.0f ? Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() : Rate);
}

// TODO: Comment
ASampleInteractableObject* ASampleCharacter::GetInteractableInView()
{
	FVector CamLocation;
	FRotator CamRotation;

	if (Controller == nullptr)
		return nullptr;

	Controller->GetPlayerViewPoint(CamLocation, CamRotation);
	const FVector TraceStart = CamLocation;
	const FVector Direction = CamRotation.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(TEXT("TraceInteractableObject"), true, this);
	TraceParams.bReturnPhysicalMaterial = false;

	// TODO: Comment
	TraceParams.bTraceComplex = false;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<ASampleInteractableObject>(Hit.GetActor());
}


void ASampleCharacter::OpenInventory()
{
	ASamplePlayerControllerBase* PlayerController = Cast<ASamplePlayerControllerBase>(GetController());	

	if (PlayerController != nullptr)
	{		
		PlayerController->ToggleInventory();		
	}
}

void ASampleCharacter::CrouchInput()
{
	if (bIsCrouched)
	{
		UnCrouch();
	} 
	else
	{
		Crouch();
	}
}

void ASampleCharacter::AimInput()
{
	if (GetLocalRole() >= ROLE_Authority)
	{
		bIsAiming = !bIsAiming;

		UCharacterMovementComponent* CharMovement = GetCharacterMovement();

		if (GetMesh() != nullptr && GetMesh()->GetAnimInstance() != nullptr 
			&& CharMovement != nullptr)
		{
			USampleAnimInstance* AnimInst = Cast<USampleAnimInstance>(GetMesh()->GetAnimInstance());
			if (AnimInst != nullptr)
			{
				if (bIsAiming)
				{
					AnimInst->bIsAiming = true;

					CharMovement->MaxWalkSpeed = AimedWalkSpeed;
					CharMovement->MaxWalkSpeedCrouched = AimedCrouchWalkSpeed;
					CharMovement->MaxAcceleration = AimedAcceleration;
					
				}
				else
				{
					AnimInst->bIsAiming = false;

					CharMovement->MaxWalkSpeed = NonAimedWalkSpeed;
					CharMovement->MaxWalkSpeedCrouched = NonAimedCrouchWalkSpeed;
					CharMovement->MaxAcceleration = NonAimedAcceleration;
				}
			}
		}
	}	
	else
	{
		ServerAim();
	}
}

FRotator ASampleCharacter::GetBaseAimRotation() const
{
	// If we have a controller, by default we aim at the player's 'eyes' direction
	// that is by default Controller.Rotation for AI, and camera (crosshair) rotation for human players.
	
	FVector POVLoc;
	FRotator POVRot;
	if (Controller != nullptr && !InFreeCam())
	{
		Controller->GetPlayerViewPoint(POVLoc, POVRot);
		return POVRot;
	}

	// If we have no controller, we simply use our rotation
	POVRot = GetActorRotation();

	// If our Pitch is 0, then use a replicated view pitch
	if (FMath::IsNearlyZero(POVRot.Pitch))
	{
		if (BlendedReplayViewPitch != 0.0f)
		{
			// If we are in a replay and have a blended value for playback, use that
			POVRot.Pitch = BlendedReplayViewPitch;
		}
		else
		{
			// Else use the RemoteViewPitch and RemoteViewYaw
			POVRot.Pitch = RemoteViewPitch;
			POVRot.Pitch = POVRot.Pitch * 360.0f / 255.0f;

			POVRot.Yaw = RemoteViewYaw;
			POVRot.Yaw = POVRot.Yaw * 360.0f / 255.0f;
		}
	}

	return POVRot;
}

void ASampleCharacter::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (GetLocalRole() == ROLE_Authority && GetController() != nullptr)
	{
		SetRemoteViewYaw(GetController()->GetControlRotation().Yaw);
	}
}

void ASampleCharacter::SetRemoteViewYaw(float NewRemoteViewYaw)
{
	// Compress yaw to 1 byte
	NewRemoteViewYaw = FRotator::ClampAxis(NewRemoteViewYaw);
	RemoteViewYaw = (uint8)(NewRemoteViewYaw * 255.f / 360.f);
}

void ASampleCharacter::ServerAim_Implementation()
{
	AimInput();
}

bool ASampleCharacter::ServerAim_Validate()
{
	return true;
}

void ASampleCharacter::OnRep_IsAiming()
{
	if (GetMesh() != nullptr && GetMesh()->GetAnimInstance() != nullptr)
	{
		USampleAnimInstance* AnimInst = Cast<USampleAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInst != nullptr)
		{
			if (bIsAiming)
			{
				AnimInst->bIsAiming = true;
			}
			else
			{
				AnimInst->bIsAiming = false;
			}
		}
	}
}

void ASampleCharacter::StartFire()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StartFire();
	}
}

void ASampleCharacter::StopFire()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StopFire();
	}
}

void ASampleCharacter::Use()
{
	// TODO: Comment
	if (GetLocalRole() >= ROLE_Authority)
	{
		ASampleInteractableObject* Interactable = GetInteractableInView();
		if (Interactable != nullptr)
		{			
			Interactable->OnUsed(this);			
		}
	}
	else
	{
		ServerUse();
	}
}

void ASampleCharacter::ServerUse_Implementation()
{
	Use();
}


bool ASampleCharacter::ServerUse_Validate()
{
	return true;
}

void ASampleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASampleCharacter, CurrentWeapon);
	DOREPLIFETIME(ASampleCharacter, bIsAiming);

	DOREPLIFETIME_CONDITION(ASampleCharacter, RemoteViewYaw, COND_SkipOwner);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Items/SampleWeapon.h"
#include "GameFramework/Player/SampleCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "SampleProject/SampleProject.h"

// Sets default values
ASampleWeapon::ASampleWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	if (WeaponMesh != nullptr)
	{
		RootComponent = WeaponMesh;
	}

	MuzzleSocketName = TEXT("MuzzleSocket");
	TracerTargetName = TEXT("Target");

	BaseDamage = 20.0f;
	BulletSpread = 1.0f;
	WeaponRange = 15000;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASampleWeapon::BeginPlay()
{
	Super::BeginPlay();	
}

// TODO : Adjust aim
void ASampleWeapon::Fire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}

	ASampleCharacter* MyOwner = Cast<ASampleCharacter>(GetOwner());

	if (MyOwner != nullptr && MyOwner->GetInstigatorController() != nullptr)
	{
		FVector EyeLocation = FVector::ZeroVector;
		FRotator EyeRotation = FRotator::ZeroRotator;

		FVector ShotDirection = GetShotDirection(MyOwner, EyeLocation, EyeRotation);

		// BulletSpread
		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		FVector TraceEnd = EyeLocation + (ShotDirection * WeaponRange);

		// Particle "Target" parameter
		FVector TracerEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult ShotHit = LineSingleWeaponTrace(MyOwner, EyeLocation, TraceEnd);
		CheckShotHit(ShotHit, MyOwner, ShotDirection, SurfaceType, TracerEndPoint);

		PlayFireEffects(TracerEndPoint);

		if (GetLocalRole() >= ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}
	}
}

void ASampleWeapon::CheckShotHit(const FHitResult& InHitResult, const ASampleCharacter* InOwnerCharacter, const FVector& HitFromDirection, EPhysicalSurface OutPhysicalSurface , FVector& InTracerEndPoint)
{
	if (InOwnerCharacter != nullptr)
	{
		if (InHitResult.bBlockingHit)
		{
			// Blocking hit! Process damage
			AActor* HitActor = InHitResult.GetActor();

			OutPhysicalSurface = UPhysicalMaterial::DetermineSurfaceType(InHitResult.PhysMaterial.Get());
			UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage, HitFromDirection, InHitResult, InOwnerCharacter->GetInstigatorController(), (AActor*)InOwnerCharacter, DamageType);
			PlayImpactEffects(OutPhysicalSurface, InHitResult.ImpactPoint);
			InTracerEndPoint = InHitResult.ImpactPoint;
		}
	}
}

FHitResult ASampleWeapon::LineSingleWeaponTrace(const ASampleCharacter* InOwnerCharacter, const FVector& TraceFrom, const FVector& TraceTo) const
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InOwnerCharacter);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, QueryParams);

	return Hit;
}

FVector ASampleWeapon::GetShotDirection(const ASampleCharacter* InOwnerCharacter, FVector& OutLocation, FRotator& OutRotation) const
{
	FVector FinalShotDirection = FVector::ZeroVector;

	if (InOwnerCharacter != nullptr && InOwnerCharacter->GetInstigatorController() != nullptr)
	{
		FVector EyeLocation = FVector::ZeroVector;
		FRotator EyeRotation = FRotator::ZeroRotator;

		InOwnerCharacter->GetInstigatorController()->GetPlayerViewPoint(EyeLocation, EyeRotation);

		OutLocation = EyeLocation;
		OutRotation = EyeRotation;

		FinalShotDirection = EyeRotation.Vector();
	}

	return FinalShotDirection;
}

FVector ASampleWeapon::GetMuzzleLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FVector ASampleWeapon::GetMuzzleDirection() const
{
	return WeaponMesh->GetSocketRotation(MuzzleSocketName).Vector();
}

void ASampleWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASampleWeapon::ServerFire_Validate()
{
	return true;
}

void ASampleWeapon::StartFire()
{
	Fire();
}

void ASampleWeapon::StopFire()
{
}

void ASampleWeapon::OnRep_HitScanTrace()
{
	// Play cosmetic FX
	PlayFireEffects(HitScanTrace.TraceTo);

	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}


void ASampleWeapon::PlayFireEffects(FVector TraceEnd)
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		ASampleCharacter* MyOwner = Cast<ASampleCharacter>(GetOwner());
		if (MyOwner != nullptr)
		{
			if (MyOwner->IsAiming())
			{
				MyOwner->PlayAnimMontage(FireIronsightsMontage);
			}
			else
			{
				MyOwner->PlayAnimMontage(FireMontage);
			}
		}
	}

	if (ShotFireSound != nullptr && WeaponMesh != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShotFireSound, GetMuzzleLocation());
	}

	if (MuzzleEffect != nullptr)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMesh, MuzzleSocketName);
	}

	if (ShotFireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShotFireSound, WeaponMesh->GetSocketLocation(MuzzleSocketName));
	}


	if (TracerEffect != nullptr)
	{
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);

		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}

	APawn* MyPawnOwner = Cast<APawn>(GetOwner());
	if (MyPawnOwner != nullptr)
	{
		APlayerController* PC = Cast<APlayerController>(MyPawnOwner->GetController());
		if (PC != nullptr && FireCamShake != nullptr)
		{
			PC->ClientStartCameraShake(FireCamShake);
		}
	}
}

void ASampleWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* SelectedEfffect = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:	
		SelectedEfffect = FleshImpactEffect;
		break;
	default:
		SelectedEfffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEfffect != nullptr)
	{
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEfffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void ASampleWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASampleWeapon, HitScanTrace, COND_SkipOwner);
}


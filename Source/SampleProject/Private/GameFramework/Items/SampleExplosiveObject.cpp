// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Items/SampleExplosiveObject.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/Components/SampleHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Items/SampleEffectActor.h"
#include "Engine/World.h"
#include "SampleProject/SampleProject.h"

// Sets default values
ASampleExplosiveObject::ASampleExplosiveObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	if (MeshComp != nullptr)
	{
		MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
		MeshComp->SetSimulatePhysics(true);
		RootComponent = MeshComp;
	}

	HealthComp = CreateDefaultSubobject<USampleHealthComponent>(TEXT("HealthComp"));
	if (HealthComp !=nullptr)
	{
		HealthComp->SetHealth(70);
	}

	// For RadialForceComponent 
	RadialForceStrength = 100;
	RadialRadius = 360;
	RadialImpulseStrength = 400;

	// For AddImpulse 
	LaunchStrength = 450;

	// For ApplyRadialDamage
	BaseDamage = 60;
	DamageRadius = 360;

	//SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	SetReplicates(true);
	SetReplicateMovement(true);
}



// Called when the game starts or when spawned
void ASampleExplosiveObject::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComp != nullptr)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &ASampleExplosiveObject::OnHealthChanged);
	}
}

void ASampleExplosiveObject::OnHealthChanged(USampleHealthComponent* BarrelHealtComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bExploded)
	{
		// Already exploded, nothing to do.
		return;
	}

	if (Health <= 0 && !bExploded)
	{
		/* Explode!
		Doing that on server-side to gain more performance on clients sides
		*/
		bExploded = true;
		OnRep_Exploded();

		// Apply damage to players, enemies and environmental objects
		TArray<AActor*> IgnoreActors;
		UGameplayStatics::ApplyRadialDamage(this, BaseDamage, GetActorLocation(), DamageRadius, BarrelDamageType, IgnoreActors, this, NULL);

		if (MeshComp != nullptr && MeshComp->IsSimulatingPhysics())
		{
			// Boost the barrel upwards
			FVector LaunchDirection = GetActorUpVector();
			FVector LaunchVelocity = LaunchDirection * LaunchStrength;
			MeshComp->AddImpulse(LaunchVelocity, NAME_None, false);

			// RadialForceComponent dynamically declared 
			URadialForceComponent* RadialForceComp = NewObject<URadialForceComponent>(this);
			if (ensure(RadialForceComp))
			{
				RadialForceComp->SetupAttachment(RootComponent);
				RadialForceComp->SetWorldTransform(MeshComp->GetComponentTransform());
				RadialForceComp->RegisterComponent();
				RadialForceComp->bIgnoreOwningActor = true;
				RadialForceComp->bImpulseVelChange = true;

				RadialForceComp->Radius = RadialRadius;
				RadialForceComp->ForceStrength = RadialForceStrength;
				RadialForceComp->DestructibleDamage = RadialForceStrength;
				RadialForceComp->ImpulseStrength = RadialImpulseStrength;

				/* Blast away nearby physics actors
				REMEMBER: Set  Static Mesh Replicate Movement to TRUE in the Editor, anyway it wont work on CLIENT SIDE
					or move RadialForceComp dynamic declaration to OnExplode() function
				*/
				RadialForceComp->FireImpulse();

				RadialForceComp->UnregisterComponent();
				RadialForceComp->DestroyComponent(true);
			}
		}
	}
}

void ASampleExplosiveObject::OnRep_Exploded()
{
	// Play FX after explosion
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation());
	UGameplayStatics::SpawnSoundAttached(ExplosionFXSound, RootComponent);

	if (MeshComp != nullptr && ExplodedMaterial != nullptr)
	{
		// Changing material to blackened version after explosion
		MeshComp->SetMaterial(0, ExplodedMaterial);
	}
	
	
	if (EffectActorClass != nullptr)
	{
		FVector LocationToSpawn = GetActorLocation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		EffectActorClassToSpawn = GetWorld()->SpawnActor<ASampleEffectActor>(EffectActorClass, LocationToSpawn, FRotator::ZeroRotator, SpawnParams);

		if (EffectActorClassToSpawn != nullptr && MeshComp != nullptr)
		{
			EffectActorClassToSpawn->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepWorldTransform);
		}
	}	
}

// Called every frame
void ASampleExplosiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASampleExplosiveObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASampleExplosiveObject, bExploded);
}

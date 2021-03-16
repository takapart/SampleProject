// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Items/SampleDestructibleObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Components/SampleHealthComponent.h"
#include "SampleProject/SampleProject.h"

// Sets default values
ASampleDestructibleObjectBase::ASampleDestructibleObjectBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	if (MeshComp != nullptr)
	{
		RootComponent = MeshComp;
	}

	DestroyedMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestroyedMeshComp"));
	if (DestroyedMeshComp != nullptr)
	{
		DestroyedMeshComp->SetupAttachment(RootComponent);
		DestroyedMeshComp->SetVisibility(false);
	}

	HealthComp = CreateDefaultSubobject<USampleHealthComponent>(TEXT("HealthComp"));

	//SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASampleDestructibleObjectBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComp != nullptr)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &ASampleDestructibleObjectBase::OnHealthChanged);
	}
}

// Called every frame
void ASampleDestructibleObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASampleDestructibleObjectBase::OnHealthChanged(USampleHealthComponent* ObjectHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bDestroyed)
	{
		return;
	}

	if (Health <= 0.f && !bDestroyed)
	{
		bDestroyed = true;
		OnRep_Destroyed();
	}
}

void ASampleDestructibleObjectBase::OnRep_Destroyed()
{
	// Play FX after being destroyed
	if (DestroyFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyFX, GetActorLocation());
		DestroyFX->bAutoDeactivate = true;
	}

	if (DestroyFXSound != nullptr)
	{
		UGameplayStatics::SpawnSoundAttached(DestroyFXSound, RootComponent);
	}
	
	if (MeshComp !=nullptr)
	{
		MeshComp->SetVisibility(false);
	}

	if (DestroyedMeshComp != nullptr)
	{
		DestroyedMeshComp->SetVisibility(true);
	}

	if (DestroyedMaterial != nullptr && MeshComp != nullptr)
	{
		MeshComp->SetMaterial(0, DestroyedMaterial);
	}	
}


void ASampleDestructibleObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASampleDestructibleObjectBase, bDestroyed);
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Items/SamplePickupObject.h"
#include "GameFramework/Player/SampleCharacter.h"
#include "GameFramework/Player/SamplePlayerControllerBase.h"
#include "SampleProject/SampleProject.h"


ASamplePickupObject::ASamplePickupObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Ignore);

	bIsActive = false;
	bStartActive = true;
	bAllowRespawn = true;
	RespawnDelay = 5.0f;
	RespawnDelayRange = 5.0f;

	SetReplicates(true);
}

void ASamplePickupObject::BeginPlay()
{
	Super::BeginPlay();

	{
		RespawnPickup();
	}
}

void ASamplePickupObject::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	ASamplePlayerControllerBase* PlayerController = Cast<ASamplePlayerControllerBase>(InstigatorPawn->GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->AddItemToInventoryByID(ItemID);
	}

	bIsActive = false;
	OnPickedUp();

	if (bAllowRespawn)
	{
		FTimerHandle RespawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ASamplePickupObject::RespawnPickup, RespawnDelay + FMath::RandHelper(RespawnDelayRange), false);
	}
	else
	{
		// TODO: Comment
		Destroy();
	}

	ASampleCharacter* Player = Cast<ASampleCharacter>(InstigatorPawn);
}

void ASamplePickupObject::OnPickedUp()
{
	if (MeshComp != nullptr)
	{
		MeshComp->SetVisibility(false);
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASamplePickupObject::RespawnPickup()
{
	bIsActive = true;
	OnRespawned();
}

void ASamplePickupObject::OnRespawned()
{
	if (MeshComp != nullptr)
	{
		MeshComp->SetVisibility(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}


void ASamplePickupObject::OnRep_IsActive()
{
	if (bIsActive)
	{
		OnRespawned();
	}
	else
	{
		OnPickedUp();
	}
}

void ASamplePickupObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASamplePickupObject, bIsActive);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Items/SampleWeaponPickup.h"
#include "GameFramework/Player/SampleCharacter.h"

ASampleWeaponPickup::ASampleWeaponPickup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowRespawn = false;

	SetReplicatingMovement(true);
}

void ASampleWeaponPickup::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);
}
	
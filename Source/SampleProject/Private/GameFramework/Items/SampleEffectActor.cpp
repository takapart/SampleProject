// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Items/SampleEffectActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "SampleProject/SampleProject.h"


// Sets default values
ASampleEffectActor::ASampleEffectActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EffectsComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectsComp"));
	if (EffectsComp != nullptr)
	{
		EffectsComp->SetupAttachment(RootComponent);
	}
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	if (AudioComp != nullptr)
	{
		AudioComp->SetupAttachment(RootComponent);
	}

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASampleEffectActor::BeginPlay()
{
	Super::BeginPlay();	
}



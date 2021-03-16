// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Player/SampleBaseCharacter.h"

// Sets default values
ASampleBaseCharacter::ASampleBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASampleBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASampleBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




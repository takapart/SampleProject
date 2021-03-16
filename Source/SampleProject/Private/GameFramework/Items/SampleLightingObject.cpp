// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Items/SampleLightingObject.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/UI/SampleWidgetComponent.h"
#include "SampleProject/SampleProject.h"

// Sets default values
ASampleLightingObject::ASampleLightingObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	if (MeshComp != nullptr)
	{
		MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Ignore);
	}

	LightIntensity = 3000.0f;

	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("SpotLightComp"));
	if (PointLightComp != nullptr)
	{
		PointLightComp->Intensity = LightIntensity;
		PointLightComp->SetSourceRadius(360.f);
		PointLightComp->SetLightColor(FLinearColor::Yellow);
		PointLightComp->SetVisibleFlag(true);
		PointLightComp->SetupAttachment(RootComponent);
	}

	Name = TEXT("Lamp");
	Action = TEXT("Turn Off ");

	bIsActive = true;

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASampleLightingObject::BeginPlay()
{
	Super::BeginPlay();	
}

void ASampleLightingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	UpdateWidgetText(bIsActive);
}

void ASampleLightingObject::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	bIsActive = !bIsActive;

	if (GetLocalRole() == ROLE_Authority)
	{
		ToggleLight(bIsActive);
	}
}

void ASampleLightingObject::ToggleLight(bool Enabled)
{	
	if (PointLightComp == nullptr)
	{
		return;
	}

	PointLightComp->SetVisibility(Enabled);

	UpdateWidgetText(Enabled);

	if (Enabled)
	{
		if (SwitchingFXSound != nullptr)
		{
			// Play FX and sound after turn on		
			UGameplayStatics::SpawnSoundAttached(SwitchingFXSound, RootComponent);
		}
	}
	else
	{
		if (SwitchingFXSound != nullptr)
		{
			// Play sound after turn off 		
			UGameplayStatics::SpawnSoundAttached(SwitchingFXSound, RootComponent);
		}
	}
}


void ASampleLightingObject::UpdateWidgetText(bool Enabled)
{
	if (WidgetComp != nullptr)
	{
		if (Enabled)
		{
			WidgetComp->ActionType = FString("Turn Off ");
		}			
		else
		{			
			WidgetComp->ActionType = FString("Turn On ");			
		}
	}
}


void ASampleLightingObject::OnRep_IsActive()
{
	ToggleLight(bIsActive);
}


void ASampleLightingObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASampleLightingObject, bIsActive);
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Items/SampleInteractableObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/UI/SampleWidgetComponent.h"
#include "SampleProject/SampleProject.h"

// Sets default values
ASampleInteractableObject::ASampleInteractableObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (MeshComp != nullptr)
	{
		RootComponent = MeshComp;
	}

	Name = TEXT("Interactable");
	Action = TEXT("Interact");

	WidgetComp = CreateDefaultSubobject<USampleWidgetComponent>(TEXT("Widget"));
	if (WidgetComp != nullptr)
	{
		WidgetComp->SetupAttachment(RootComponent);
		WidgetComp->ObjectName = Name;
		WidgetComp->ActionType = Action;
	}

	SetReplicates(true);
}

void ASampleInteractableObject::OnUsed(APawn* InstigatorPawn)
{

}


void ASampleInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetComp != nullptr)
	{
		WidgetComp->SetVisibility(false);		
		WidgetComp->ObjectName = Name;
		WidgetComp->ActionType = Action;
	}
}

void ASampleInteractableObject::OnBeginFocus()
{
	if (WidgetComp != nullptr)
	{
		WidgetComp->DisplayTextObject();
		WidgetComp->SetVisibility(true);
	}
}

void ASampleInteractableObject::OnEndFocus()
{
	if (WidgetComp != nullptr)
	{
		WidgetComp->SetVisibility(false);
	}
}

void ASampleInteractableObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASampleInteractableObject, Name);
	DOREPLIFETIME(ASampleInteractableObject, Action);
}

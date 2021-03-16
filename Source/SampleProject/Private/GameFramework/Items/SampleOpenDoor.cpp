// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Items/SampleOpenDoor.h"
#include "GameFramework/Player/SampleCharacter.h"
#include "GameFramework/UI/SampleWidgetComponent.h"
#include "GameFramework/Components/SampleHealthComponent.h"
#include "SampleProject/SampleProject.h"

ASampleOpenDoor::ASampleOpenDoor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USampleHealthComponent>(TEXT("HealthComp"));
	if (HealthComp != nullptr)
	{
		HealthComp->SetHealth(10);
	}

	Name = TEXT("Door");
	Action = TEXT("Open ");

	bIsActive = false;

	bIsDoorClosed = false;

	bOpeningDoor = false;
	bClosingDoor = false;

	DotProd = 0.0f;
	MaxDegree = 0.0f;
	AddedRotation = 0.0f;
	PosNeg = 0.0f;
	CurrentDoorRotation = 0.0f;

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ASampleOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComp != nullptr)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &ASampleOpenDoor::OnHealthChanged);
	}
}

void ASampleOpenDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateWidgetText(bIsActive);

	if (bOpeningDoor)
	{
		OpenDoor(DeltaSeconds);
	}

	if (bClosingDoor)
	{
		CloseDoor(DeltaSeconds);
	}
}

void ASampleOpenDoor::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);	

	ASampleCharacter* MyPlayer = Cast<ASampleCharacter>(InstigatorPawn);
	if (MyPlayer != nullptr && MyPlayer->GetCameraComponent() != nullptr)
	{
		OpenVector = MyPlayer->GetCameraComponent()->GetForwardVector();
	}

	bIsActive = !bIsActive;

	ToggleDoor(bIsActive);
}

void ASampleOpenDoor::OnHealthChanged(USampleHealthComponent* ObjectHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
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

void ASampleOpenDoor::SetDestroyedWidgetText()
{
	if (WidgetComp != nullptr)
	{
		WidgetComp->ObjectName = FString("");
		WidgetComp->ActionType = FString("");
		WidgetComp->RemoveFromRoot();
	}
}

void ASampleOpenDoor::OnRep_Destroyed()
{
	//SetDestroyedWidgetText();

	// Play FX after explosion
	if (GetWorld() != nullptr && DestroyFX != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyFX, GetActorLocation());

	}

	if (DestroyFXSound != nullptr)
	{
		UGameplayStatics::SpawnSoundAttached(DestroyFXSound, RootComponent);
	}

	if (MeshComp != nullptr && DestroyedMaterial != nullptr)
	{
		MeshComp->SetMaterial(0, DestroyedMaterial);
	}
}

void ASampleOpenDoor::ToggleDoor(bool Status)
{
	if (MeshComp != nullptr)
	{
		DotProd = FVector::DotProduct(MeshComp->GetForwardVector(), OpenVector);

		PosNeg = FMath::Sign(DotProd);

		MaxDegree = PosNeg * 90.0f;

		if (bIsDoorClosed)
		{
			bIsDoorClosed = false;
			bClosingDoor = false;
			bOpeningDoor = true;
		}
		else
		{
			bOpeningDoor = false;
			bIsDoorClosed = true;
			bClosingDoor = true;
		}

		UpdateWidgetText(bIsActive);
	}
}

void ASampleOpenDoor::OpenDoor(float dt)
{
	if (MeshComp != nullptr)
	{
		CurrentDoorRotation = MeshComp->GetRelativeRotation().Yaw;

		AddedRotation = PosNeg * dt * 80;

		if (FMath::IsNearlyEqual(CurrentDoorRotation, MaxDegree, 1.5f))
		{
			bClosingDoor = false;
			bOpeningDoor = false;
		}
		else if (bOpeningDoor)
		{
			FRotator NewRotation = FRotator(0.0f, AddedRotation, 0.0f);
			MeshComp->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
		}
	}
}

void ASampleOpenDoor::CloseDoor(float dt)
{
	if (MeshComp != nullptr)
	{
		CurrentDoorRotation = MeshComp->GetRelativeRotation().Yaw;

		if (CurrentDoorRotation > 0)
		{
			AddedRotation = -dt * 80;
		}
		else
		{
			AddedRotation = dt * 80;
		}

		if (FMath::IsNearlyEqual(CurrentDoorRotation, 0.0f, 1.5f))
		{
			bClosingDoor = false;
			bOpeningDoor = false;
		}
		else if (bClosingDoor)
		{
			FRotator NewRotation = FRotator(0.0f, AddedRotation, 0.0f);
			MeshComp->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
		}
	}
}

void ASampleOpenDoor::UpdateWidgetText(bool Status)
{
	if (WidgetComp != nullptr)
	{
		if (Status)
		{
			WidgetComp->ActionType = FString("Open ");			
		}
		else
		{
			WidgetComp->ActionType = FString("Close ");
		}
	}
}

void ASampleOpenDoor::OnRep_IsActive()
{
	ToggleDoor(bIsActive);
}

void ASampleOpenDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASampleOpenDoor, bIsActive);
	DOREPLIFETIME(ASampleOpenDoor, bDestroyed);
}
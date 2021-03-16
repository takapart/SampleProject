// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Components/SampleHealthComponent.h"
#include "SampleProject/SampleProject.h"

// Sets default values for this component's properties
USampleHealthComponent::USampleHealthComponent()
{
	DefaultHealth = 100.f;

	bIsDead = false;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USampleHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Comment
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USampleHealthComponent::HandleTakeAnyDamage);
		}
	}

	// TODO: Comment
	Health = DefaultHealth;
}

void USampleHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

void USampleHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	// TODO: Comment
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void USampleHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USampleHealthComponent, Health);
}
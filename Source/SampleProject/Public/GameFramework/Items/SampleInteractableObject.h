// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SampleInteractableObject.generated.h"

UCLASS()
class SAMPLEPROJECT_API ASampleInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	// Variables

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Display")
	FString Name;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Display")
	FString Action;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USampleWidgetComponent* WidgetComp;

public: // Methods

	// Sets default values for this actor's properties
	ASampleInteractableObject(const FObjectInitializer& ObjectInitializer);

protected: // Methods

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	// Methods
	
	virtual void OnBeginFocus();

	virtual void OnEndFocus();

	virtual void OnUsed(APawn* InstigatorPawn);

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return MeshComp; }
};


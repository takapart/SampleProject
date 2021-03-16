// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "SampleUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwningActorChanged, AActor*, NewOwner);

/**
 * Extended version of UserWidget, connected by dynamic delegate with Widget Component to helps display Objects names and potentially actions to take
 */
UCLASS(Abstract)
class SAMPLEPROJECT_API USampleUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected: // Variables

	// OwningActor of Widget Component
	UPROPERTY(BlueprintReadOnly, Category = "ActorWidget")
	AActor* OwningActor;

	// For casting to get variables and functions of PlayerController at runtime in BP versions of Widget
	UPROPERTY(BlueprintReadWrite)
	class ASamplePlayerControllerBase* PlayerController;

	// For casting to get variables and functions of Player at runtime in BP versions of Widget
	UPROPERTY(BlueprintReadWrite)
	class ASampleCharacter* Player;
	
public: // Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DisplayText;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnOwningActorChanged OnOwningActorChanged;

public: // Methods

	// Setting Owning Actor of Widget Component when Player is looking at interactable item
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetOwningActor(AActor* NewOwner);

	// Overriding for casting to get variables and functions of Player and PlayerController at runtime in BP versions of Widget
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};



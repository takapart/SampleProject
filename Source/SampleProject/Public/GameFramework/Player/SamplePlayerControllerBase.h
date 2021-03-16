// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "SamplePlayerControllerBase.generated.h"


/**
 * Player Controller class connected with Player's inventory 
 */

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()

public:

	FInventoryItem()
	{
		ItemName = FText::FromString("Item");
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	bool operator==(const FInventoryItem& OtherItem) const 
	{
		if (ItemID == OtherItem.ItemID)
		{
			return true;
		}

		return false;
	}
};

UCLASS()
class SAMPLEPROJECT_API ASamplePlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public: // Variables


	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Inventory")
	bool bIsInventoryOpen;


	UPROPERTY(BlueprintReadWrite)
	class ASampleHUD* MySampleHUD;

	// Array of Player's Inventory to store items
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Inventory Manager")
	TArray<FInventoryItem> Inventory;

	// Connected variables with UW3UserWidget to get proper information about items, which player has taken 

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText SetFirstItemName;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	UTexture2D* SetFirstItemThumbnail;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText SetSecondItemName;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	UTexture2D* SetSecondItemThumbnail;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText InventoryHint;
	

public: // Methods

	ASamplePlayerControllerBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void AddItemToInventoryByID(FName ID);

	UFUNCTION(BlueprintCallable, Category = "Inventory Manager")
	void ToggleInventory();

	virtual void BeginPlay() override;
};



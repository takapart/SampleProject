// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Player/SamplePlayerControllerBase.h"
#include "SampleProject/SampleProject.h"
#include "GameFramework/World/SampleGameMode.h"
#include "GameFramework/UI/SampleHUD.h"
#include "GameFramework/UI/SampleUserWidget.h"

ASamplePlayerControllerBase::ASamplePlayerControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InventoryHint = FText::FromString("Inventory");

	bIsInventoryOpen = false;	
}

void ASamplePlayerControllerBase::AddItemToInventoryByID_Implementation(FName ID)
{	
	ASampleGameMode* GameMode = Cast<ASampleGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		UDataTable* ItemTable = GameMode->GetItemDB();
		if (ItemTable != nullptr)
		{
			FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

			if (ItemToAdd != nullptr)
			{
				Inventory.Add(*ItemToAdd);

				if (Inventory.Num() > 0)
				{
					SetFirstItemName = Inventory[Inventory.Num() - 1].ItemName;
					SetFirstItemThumbnail = Inventory[Inventory.Num() - 1].Thumbnail;

					InventoryHint = FText::FromString("You got one item");
				}
				if (Inventory.Num() > 1)
				{
					SetFirstItemName = Inventory[Inventory.Num() - 1].ItemName;
					SetFirstItemThumbnail = Inventory[Inventory.Num() - 1].Thumbnail;

					SetSecondItemName = Inventory[Inventory.Num() - 2].ItemName;
					SetSecondItemThumbnail = Inventory[Inventory.Num() - 2].Thumbnail;

					InventoryHint = FText::FromString("You got two items");
				}
				if (Inventory.Num() > 2)
				{
					InventoryHint = FText::FromString("Press I to open Inventory");
				}
			}
		}
	}	
}


void ASamplePlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	MySampleHUD = Cast<ASampleHUD>(GetHUD());
}

bool ASamplePlayerControllerBase::AddItemToInventoryByID_Validate(FName ID)
{
	return true;
}

void ASamplePlayerControllerBase::ToggleInventory()
{	
	if (MySampleHUD != nullptr)
	{
		if (bIsInventoryOpen)
		{
			bIsInventoryOpen = false;			
			
			if (MySampleHUD->InventoryWidget != nullptr)
 			{
				MySampleHUD->InventoryWidget->RemoveFromViewport();
 			}

			// TODO: Comment
			if (MySampleHUD->InventorySmallWidget != nullptr)
			{
				MySampleHUD->InventorySmallWidget->AddToViewport();
			}

			FInputModeGameOnly InputGameMode;

			SetInputMode(InputGameMode);
			bShowMouseCursor = false;			
		}
		else
		{
			bIsInventoryOpen = true;

			// TODO: Comment
			if (MySampleHUD->InventorySmallWidget != nullptr)
			{
				MySampleHUD->InventorySmallWidget->RemoveFromViewport();
			}


			if (MySampleHUD->InventoryWidget != nullptr)
			{
				MySampleHUD->InventoryWidget->AddToViewport();
			}

			FInputModeUIOnly InputModeUIOnly;

			//if (MyWW3HUD->InventoryWidget)
			//InputModeUIOnly.SetWidgetToFocus(MyWW3HUD->InventoryWidget->TakeWidget());

			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			SetInputMode(InputModeUIOnly);
			bShowMouseCursor = true;			
		}		
	}
	else
	{
		MySampleHUD = Cast<ASampleHUD>(GetHUD());

		if (MySampleHUD != nullptr)
		{
			ToggleInventory();
		}
	}
}

void ASamplePlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASamplePlayerControllerBase, Inventory);
	DOREPLIFETIME(ASamplePlayerControllerBase, bIsInventoryOpen);
	DOREPLIFETIME(ASamplePlayerControllerBase, SetFirstItemName);
	DOREPLIFETIME(ASamplePlayerControllerBase, SetFirstItemThumbnail);
	DOREPLIFETIME(ASamplePlayerControllerBase, SetSecondItemName);
	DOREPLIFETIME(ASamplePlayerControllerBase, SetSecondItemThumbnail);
	DOREPLIFETIME(ASamplePlayerControllerBase, InventoryHint);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleProjectGameModeBase.h"
#include "GameFramework/Player/SampleCharacter.h"
#include "GameFramework/Player/SamplePlayerControllerBase.h"
#include "GameFramework/UI/SampleHUD.h"

ASampleProjectGameModeBase::ASampleProjectGameModeBase()
{
	// Setting Default classess for our game

	DefaultPawnClass = ASampleCharacter::StaticClass();
	PlayerControllerClass = ASamplePlayerControllerBase::StaticClass();
	HUDClass = ASampleHUD::StaticClass();
}

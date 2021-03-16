// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/World/SampleGameMode.h"
#include "GameFramework/Player/SampleCharacter.h"
#include "GameFramework/Player/SamplePlayerControllerBase.h"
#include "GameFramework/UI/SampleHUD.h"


ASampleGameMode::ASampleGameMode()
{
	DefaultPawnClass = ASampleCharacter::StaticClass();
	PlayerControllerClass = ASamplePlayerControllerBase::StaticClass();
	HUDClass = ASampleHUD::StaticClass();
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGMFishingGameMode.h"
#include "GGMFishingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGGMFishingGameMode::AGGMFishingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

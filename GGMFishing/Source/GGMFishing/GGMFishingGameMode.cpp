// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGMFishingGameMode.h"
#include "TopDownCameraPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/LevelScriptActor.h"

AGGMFishingGameMode::AGGMFishingGameMode()
{
	DefaultPawnClass = ATopDownCameraPawn::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Script/GGMFishing.TopDownCameraPawn"));
	if (PlayerPawnClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
}

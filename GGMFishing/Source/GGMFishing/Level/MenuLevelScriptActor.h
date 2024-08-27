// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MenuLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class GGMFISHING_API AMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	AMenuLevelScriptActor();

public:
	void BeginPlay() override;

private:
	UPROPERTY()
	TSubclassOf<UUserWidget> MenuWidgetClass;
	TObjectPtr<UUserWidget> MenuWidget;
};

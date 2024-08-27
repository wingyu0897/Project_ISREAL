// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/MenuLevelScriptActor.h"
#include "Blueprint/UserWidget.h"

AMenuLevelScriptActor::AMenuLevelScriptActor()
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetClassRef(TEXT("/Game/GGMFishing/UI/MainMenuWidget.MainMenuWidget_C"));
	if (MenuWidgetClassRef.Class != nullptr)
	{
		MenuWidgetClass = MenuWidgetClassRef.Class;
	}
}

void AMenuLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (IsValid(MenuWidgetClass))
		{
			MenuWidget = CreateWidget(World, MenuWidgetClass);

			if (IsValid(MenuWidget))
			{
				MenuWidget->AddToViewport();
			}
		}
	}
}

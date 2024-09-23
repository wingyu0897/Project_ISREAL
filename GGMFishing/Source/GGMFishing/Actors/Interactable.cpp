// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactable.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	//Cube->SetupAttachment(RootComponent);
	//ConstructorHelpers::FObjectFinder<UStaticMesh> CubeRef = TEXT("/Engine/BasicShapes/Cube.Cube");
	//if (CubeRef.Object != nullptr)
	//{
	//	Cube->SetStaticMesh(CubeRef.Object);
	//}

	// Sphere
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetRelativeScale3D(FVector(9.f, 9.f, 9.f));
	Sphere->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	
	// Widget
	InteractableWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interactable"));
	InteractableWidget->SetupAttachment(Sphere);
	ConstructorHelpers::FClassFinder<UUserWidget> WidgetRef = TEXT("/Game/GGMFishing/UI/InteractionWidget.InteractionWidget_C");
	if (WidgetRef.Class != nullptr)
	{
		InteractableWidget->SetWidgetClass(WidgetRef.Class);
	}
	InteractableWidget->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	InteractableWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractableWidget->SetVisibility(false);
}

void AInteractable::ShowWidget()
{
	InteractableWidget->SetVisibility(true);
}

void AInteractable::HideWidget()
{
	InteractableWidget->SetVisibility(false);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FishingRod.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/SceneComponent.h"
#include "../Datas/CustomStructs.h"
#include "../UI/InteractionWidget.h"
#include "../UI/FishingWidget.h"
#include "TopDownCameraPawn.h"

AFishingRod::AFishingRod()
{
	// Fish Data Table
	ConstructorHelpers::FObjectFinder<UDataTable> FishDataTableRef(TEXT("/Game/GGMFishing/Datas/DT_Fish.DT_Fish"));
	if (FishDataTableRef.Succeeded())
	{
		FishDataTable = FishDataTableRef.Object;
	}

	// Widget
	NotifyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NotifyWidget"));
	NotifyWidget->SetupAttachment(Sphere);
	NotifyWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	NotifyWidget->SetWidgetSpace(EWidgetSpace::Screen);
	NotifyWidget->SetVisibility(false);

	ConstructorHelpers::FClassFinder<UUserWidget> NotifyWidgetRef(TEXT("/Game/GGMFishing/UI/WBP_FishingRodNotify.WBP_FishingRodNotify_C"));
	if (NotifyWidgetRef.Succeeded())
	{
		NotifyWidget->SetWidgetClass(NotifyWidgetRef.Class);
	}

	FishingProgressWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FishingProgressWidget"));
	FishingProgressWidget->SetupAttachment(Sphere);
	FishingProgressWidget->SetRelativeLocation(FVector(0.f, 75.f, 100.f));
	FishingProgressWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FishingProgressWidget->SetVisibility(false);

	ConstructorHelpers::FClassFinder<UUserWidget> FishingProgressWidgetRef(TEXT("/Game/GGMFishing/UI/WBP_Fishing.WBP_Fishing_C"));
	if (FishingProgressWidgetRef.Succeeded()) 
	{
		FishingProgressWidget->SetWidgetClass(FishingProgressWidgetRef.Class);
	}

	// Camera
	FishingCameraPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FishingCamPos"));
	FishingCameraPosition->SetupAttachment(Sphere);
	FishingCameraPosition->SetRelativeLocation(FVector(-500.f, 0.f, 600.f));
	FishingCameraPosition->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));

	PrimaryActorTick.bCanEverTick = true;
}

void AFishingRod::BeginPlay()
{
	Super::BeginPlay();
	SelectNextFishData();
	Casting();

	UInteractionWidget* widget = Cast<UInteractionWidget>(InteractableWidget->GetWidget());
	if (ensure(widget))
	{
		Cast<UTextBlock>(widget->GetWidgetFromName(TEXT("TxtInteraction")))->SetText(FText::FromString(TEXT("Catch")));
		//widget->UpdateInteractionText(TEXT("Catch"));
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *NextFishData.FishName.ToString());
}

void AFishingRod::Tick(float DeltaTime)
{
	if (bIsCatching)
	{
		Catching(DeltaTime);
	}
}

void AFishingRod::Interact(AActor* InteractActor)
{
	Super::Interact(InteractActor);

	PlayerActor = Cast<ATopDownCameraPawn>(InteractActor);

	HideWidget();

	if (bIsCatching)
	{
		TargetSliderValue = FMath::Clamp(TargetSliderValue + 0.1f, 0.f, 1.f);
		return;
	}

	if (bIsCasting)
	{
		if (bIsBiting)
			TryCatch();
		else
			FailFishing();
	}
	else
	{
		FailFishing();
	}
}

void AFishingRod::SelectNextFishData()
{
	TArray Names = FishDataTable->GetRowNames();
	NextFishData = *(FishDataTable->FindRow<FFishData>(Names[FMath::RandRange(0, Names.Num() - 1)], TEXT("")));
}

void AFishingRod::SetFishSliderValue()
{
	FishSliderTargetValue = FMath::RandRange(0.f, 1.f);

	float FishMoveTime = FMath::RandRange(1.f, 3.f);

	GetWorldTimerManager().ClearTimer(FishMoveTimer);
	GetWorldTimerManager().SetTimer(FishMoveTimer, this, &AFishingRod::SetFishSliderValue, FishMoveTime, false);
}

void AFishingRod::Casting()
{
	if (bIsCasting) return;

	bIsCasting = true;
	float castingTime = FMath::RandRange(2.f, 7.f);
	GetWorldTimerManager().SetTimer(Timer, this, &AFishingRod::Bite, castingTime, false);
}

void AFishingRod::Bite()
{
	bIsBiting = true;
	GetWorldTimerManager().ClearTimer(Timer);
	NotifyWidget->SetVisibility(true);

	GetWorldTimerManager().SetTimer(Timer, this, &AFishingRod::FailFishing, 2.f, false);
}

void AFishingRod::FailFishing()
{
	bIsBiting = false;
	bIsCasting = false;
	NotifyWidget->SetVisibility(false);
	FishingProgressWidget->SetVisibility(false);

	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().ClearTimer(FishMoveTimer);

	SelectNextFishData();
	Casting();

	if (bIsInteracting)
		ShowWidget();


	if (PlayerActor != nullptr)
	{
		PlayerActor->MoveCameraAttach(nullptr);
		PlayerActor->SetMoveable(true);
	}
}

void AFishingRod::TryCatch()
{
	if (PlayerActor != nullptr)
	{
		PlayerActor->MoveCameraAttach(FishingCameraPosition);
		PlayerActor->SetMoveable(false);
	}

	FishingProgressWidget->SetVisibility(true);
	NotifyWidget->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(Timer);

	bIsCatching = true;
	Progress = 0.5f;
	TargetSliderValue = 0.f;
	FishSliderValue = FMath::RandRange(0.f, 1.f);

	SetFishSliderValue();

	FishingWidget = Cast<UFishingWidget>(FishingProgressWidget->GetWidget());
	//Catch();
}

void AFishingRod::Catching(float DeltaTime)
{
	FishSliderValue = FMath::Lerp(FishSliderValue, FishSliderTargetValue, 0.1f);
	TargetSliderValue = FMath::Clamp(TargetSliderValue - DeltaTime * 0.2f, 0.f, 1.f);

	if (FMath::Abs(FishSliderValue - TargetSliderValue) < 0.1f)
	{
		Progress = FMath::Clamp(Progress + DeltaTime * 0.1f, 0.f, 1.f);
		if (Progress >= 1.f)
		{
			Catch();
		}
	}
	else
	{
		Progress = FMath::Clamp(Progress - DeltaTime * 0.1f, 0.f, 1.f);
		if (Progress <= 0.f)
		{
			FailFishing();
		}
	}

	FishingWidget->UpdateProgressBar(Progress);
	FishingWidget->UpdateSliderFish(FishSliderValue);
	FishingWidget->UpdateSliderTarget(TargetSliderValue);
}

void AFishingRod::Catch()
{
	UE_LOG(LogTemp, Log, TEXT("Cath Fish: %s"), *NextFishData.FishName.ToString());

	bIsCasting = false;
	bIsBiting = false;
	bIsCatching = false;

	SelectNextFishData();
	Casting();

	GetWorldTimerManager().ClearTimer(FishMoveTimer);

	if (bIsInteracting)
		ShowWidget();

	FishingProgressWidget->SetVisibility(false);

	if (PlayerActor != nullptr)
	{
		PlayerActor->MoveCameraAttach(nullptr);
		PlayerActor->SetMoveable(true);
	}
}

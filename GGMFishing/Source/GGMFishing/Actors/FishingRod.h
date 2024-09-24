// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactable.h"
#include "../Datas/CustomStructs.h"
#include "FishingRod.generated.h"

/**
 * 
 */
UCLASS()
class GGMFISHING_API AFishingRod : public AInteractable
{
	GENERATED_BODY()

public:
	AFishingRod();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime);

	virtual void Interact(AActor* InteractActor) override;

private:
	void SelectNextFishData();
	void SetFishSliderValue();
	void Casting();
	void Bite();
	void FailFishing();
	void TryCatch();
	void Catching(float DeltaTime);
	void Catch();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fishing)
	TObjectPtr<UDataTable> FishDataTable;
	
private:
	// Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fishing, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> NotifyWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fishing, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> FishingProgressWidget;

	UPROPERTY()
	TObjectPtr<class UFishingWidget> FishingWidget;
	
	// Fishing
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Fishing, meta = (AllowPrivateAccess = "true"))
	FFishData NextFishData;

	UPROPERTY()
	FTimerHandle Timer;
	UPROPERTY()
	FTimerHandle FishMoveTimer;

	UPROPERTY()
	TObjectPtr<class ATopDownCameraPawn> PlayerActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> FishingCameraPosition;

private:
	UPROPERTY()
	bool bIsCasting = false;
	UPROPERTY()
	bool bIsBiting = false;
	UPROPERTY()
	bool bIsCatching = false;

	UPROPERTY()
	float TargetSliderValue = 0.f;
	UPROPERTY()
	float FishSliderValue = 0.f;
	UPROPERTY()
	float FishSliderTargetValue = 0.f;
	UPROPERTY()
	float Progress = 0.5f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FishingWidget.generated.h"

/**
 * 
 */
UCLASS()
class GGMFISHING_API UFishingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFishingWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateProgressBar(float Percentage);
	void UpdateSliderFish(float Value);
	void UpdateSliderTarget(float Value);
	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> ProgressBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> SliderFish;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USlider> SliderTarget;
};

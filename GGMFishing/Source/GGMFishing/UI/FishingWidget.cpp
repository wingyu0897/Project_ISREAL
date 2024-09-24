// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FishingWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"

UFishingWidget::UFishingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UFishingWidget::NativeConstruct()
{
	ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBProgress")));
	ensure(ProgressBar);
	SliderFish = Cast<USlider>(GetWidgetFromName(TEXT("SliderFish")));
	ensure(SliderFish);
	SliderTarget = Cast<USlider>(GetWidgetFromName(TEXT("SliderTarget")));
	ensure(SliderTarget);
}

void UFishingWidget::UpdateProgressBar(float Percentage)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(Percentage);
	}
}

void UFishingWidget::UpdateSliderFish(float Value)
{
	if (SliderFish)
	{
		SliderFish->SetValue(Value);
	}
}

void UFishingWidget::UpdateSliderTarget(float Value)
{
	if (SliderTarget)
	{
		SliderTarget->SetValue(Value);
	}
}

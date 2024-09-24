// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UInteractionWidget::UInteractionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInteractionWidget::NativeConstruct()
{
	InteractionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtInteraction")));
	ensure(InteractionText);

	InteractionImage = Cast<UImage>(GetWidgetFromName(TEXT("ImgInteraction")));
	ensure(InteractionImage);
}

void UInteractionWidget::UpdateInteractionText(FString text)
{
	InteractionText->SetText(FText::FromString(text));
}

void UInteractionWidget::UpdateInteractionImage(UTexture2D* texture)
{
	InteractionImage->SetBrushFromTexture(texture);
}

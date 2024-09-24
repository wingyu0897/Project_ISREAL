// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class GGMFISHING_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInteractionWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	void UpdateInteractionText(FString text);
	void UpdateInteractionImage(UTexture2D* texture);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> InteractionText;
	UPROPERTY()
	TObjectPtr<class UImage> InteractionImage;
};

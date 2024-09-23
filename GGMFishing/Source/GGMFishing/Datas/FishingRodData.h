// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FishingRodData.generated.h"

/**
 * 
 */
UCLASS()
class GGMFISHING_API UFishingRodData : public UPrimaryDataAsset
{
	GENERATED_BODY()

//public:
//	FPrimaryAssetId GetPrimaryAssetId() const override
//	{
//		return FPrimaryAssetId("FishingRodData", GetFName());
//	}

public:
	UPROPERTY(EditAnywhere, Category = FishingRod)
	TObjectPtr<UStaticMesh> FishingRodMesh;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TopDownCameraPawn.generated.h"

UCLASS()
class GGMFISHING_API ATopDownCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopDownCameraPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Move(const struct FInputActionValue& Value);
	void CompleteMove(const struct FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess="true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess="true"))
	TObjectPtr<class UCameraComponent> TopDownCamera;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

private:
	UPROPERTY()
	FVector MoveVector;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	FVector MoveableAreaMin;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	FVector MoveableAreaMax;

	UPROPERTY()
	float Speed;
};

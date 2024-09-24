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
	virtual ~ATopDownCameraPawn() {};

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveCameraAttach(USceneComponent* InParent);
	virtual void SetMoveable(bool bInMoveable) { bIsMoveable = bInMoveable; }

private:
	void Move(const struct FInputActionValue& Value);
	void CompleteMove(const struct FInputActionValue& Value);

	void Interact(const struct FInputActionValue& Value);
	void SelectInteraction();

	UFUNCTION()
	virtual void HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Capsule, meta = (AllowPrivateAccess="true"))
	TObjectPtr<class USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess="true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess="true"))
	TObjectPtr<class UCameraComponent> TopDownCamera;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> InteractAction;

private:
	// Movement
	UPROPERTY()
	FVector MoveVector;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	FVector MoveableAreaMin;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	FVector MoveableAreaMax;
	UPROPERTY()
	float Speed;
	UPROPERTY()
	bool bIsMoveable = true;

	// Interaction
	UPROPERTY()
	TObjectPtr<class AInteractable> Interaction;
	UPROPERTY()
	TArray<class AInteractable*> Interactables;
};

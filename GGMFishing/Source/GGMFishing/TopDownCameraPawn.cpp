
#include "TopDownCameraPawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Interactable.h"

ATopDownCameraPawn::ATopDownCameraPawn()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	Sphere->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	/*Movement Settings*/
	Speed = 500.f;
	MoveVector = FVector(0.f, 0.f, 0.f);

	MoveableAreaMin = FVector(-400.f, -600.f, 0.f);
	MoveableAreaMax = FVector(400.f, 600.f, 800.f);

	/*Camera Settings*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Sphere);
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->SetWorldRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;

	/*Input Settings*/
	ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Game/GGMFishing/Input/IMC_Default.IMC_Default"));
	if (DefaultMappingContextRef.Object != nullptr)
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/GGMFishing/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionRef.Object != nullptr)
	{
		MoveAction = MoveActionRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> InteractActionRef(TEXT("/Game/GGMFishing/Input/Actions/IA_Interaction.IA_Interaction"));
	if (InteractActionRef.Object != nullptr)
	{
		InteractAction = InteractActionRef.Object;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ATopDownCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		PlayerController->SetShowMouseCursor(true);
	}

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATopDownCameraPawn::HandleOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ATopDownCameraPawn::HandleOverlapEnd);
}

void ATopDownCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextLocation = GetActorLocation() + MoveVector * Speed * DeltaTime;
	NextLocation.X = FMath::Clamp(NextLocation.X, MoveableAreaMin.X, MoveableAreaMax.X);
	NextLocation.Y = FMath::Clamp(NextLocation.Y, MoveableAreaMin.Y, MoveableAreaMax.Y);
	SetActorLocation(NextLocation);

	SelectInteraction();
}

void ATopDownCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCameraPawn::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopDownCameraPawn::CompleteMove);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ATopDownCameraPawn::Interact);
	}
}

void ATopDownCameraPawn::MoveCameraAttach(USceneComponent* InParent)
{
	if (InParent != nullptr)
	{
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
		TopDownCamera->AttachToComponent(InParent, AttachmentRules);
	}
	else
	{
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		TopDownCamera->AttachToComponent(CameraBoom, AttachmentRules, USpringArmComponent::SocketName);
		TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	}
}

void ATopDownCameraPawn::Move(const FInputActionValue& Value)
{
	if (!bIsMoveable) return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	MoveVector = FVector(MovementVector.Y, MovementVector.X, 0.f);
	MoveVector.Normalize();
}

void ATopDownCameraPawn::CompleteMove(const FInputActionValue& Value)
{
	MoveVector = FVector(0.f, 0.f, 0.f);
}

void ATopDownCameraPawn::Interact(const FInputActionValue& Value)
{
	if (Interaction == nullptr) return;

	Interaction->Interact(this);
}

void ATopDownCameraPawn::SelectInteraction()
{
	if (Interactables.IsEmpty()) return;

	if (Interaction == nullptr && Interactables.Num() > 0)
	{
		Interaction = Interactables[0];
	}

	float MinDistance = GetDistanceTo(Cast<AActor>(Interaction));

	for (int32 i = 0; i < Interactables.Num(); ++i)
	{
		float Distance = GetDistanceTo(Cast<AActor>(Interactables[i]));
		if (Distance < MinDistance)
		{
			Interaction->HideWidget();
			Interaction = Interactables[i];
			MinDistance = Distance;
		}
	}

	Interaction->ShowWidget();
}

void ATopDownCameraPawn::HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInteractable* Interacted = Cast<AInteractable>(OtherActor);
	if (Interacted)
	{
		UE_LOG(LogTemp, Log, TEXT("Interacted"));
		Interactables.Emplace(Interacted);
	}
	else
		UE_LOG(LogTemp, Log, TEXT("Fail Interacted"));
}

void ATopDownCameraPawn::HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AInteractable* Interacted = Cast<AInteractable>(OtherActor);
	if (Interacted)
	{
		if (Interactables.Contains(Interacted))
		{
			if (Interacted == Interaction)
				Interacted->HideWidget();

			Interactables.Remove(Interacted);
		}
	}
}


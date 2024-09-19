
#include "TopDownCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PawnMovementComponent.h"

ATopDownCameraPawn::ATopDownCameraPawn()
{
	/*Movement Settings*/
	Speed = 500.f;
	MoveVector = FVector(0.f, 0.f, 0.f);

	MoveableAreaMin = FVector(-400.f, -600.f, 0.f);
	MoveableAreaMax = FVector(400.f, 600.f, 800.f);

	/*Camera Settings*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->SetWorldRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;

	/*Input Settings*/
	ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Game/ThirdPerson/Input/IMC_Default.IMC_Default"));
	if (DefaultMappingContextRef.Object != nullptr)
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionRef.Object != nullptr)
	{
		MoveAction = MoveActionRef.Object;
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
}

void ATopDownCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextLocation = GetActorLocation() + MoveVector * Speed * DeltaTime;
	NextLocation.X = FMath::Clamp(NextLocation.X, MoveableAreaMin.X, MoveableAreaMax.X);
	NextLocation.Y = FMath::Clamp(NextLocation.Y, MoveableAreaMin.Y, MoveableAreaMax.Y);
	SetActorLocation(NextLocation);
}

void ATopDownCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCameraPawn::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopDownCameraPawn::CompleteMove);
	}
}

void ATopDownCameraPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	MoveVector = FVector(MovementVector.Y, MovementVector.X, 0.f);
	MoveVector.Normalize();
}

void ATopDownCameraPawn::CompleteMove(const FInputActionValue& Value)
{
	MoveVector = FVector(0.f, 0.f, 0.f);
}


// All this belongs to ApocAlypsE, the genius.

#include "PlayerTbsPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputModifiers.h"

// Sets default values
APlayerTbsPawn::APlayerTbsPawn() {
    PrimaryActorTick.bCanEverTick = true;

    // Create a root component
    USceneComponent *Root =
        CreateDefaultSubobject<USceneComponent>("RootComponent");
    SetRootComponent(Root);

    // Set defaults; TODO: Make an .ini configuration file
    ArmLength = 700.0f;
    ArmLengthChangeSpeed = 40.f;
    CameraMovementSpeed = 40.f;
    CameraRotationSpeed = 10.f;
    MinZoom = 100.f;
    MaxZoom = 1000.f;
    InterpSpeedZoom = 2.f;
    InterpSpeedLocation = 5.f;
    InterpSpeedRotation = 5.f;
    bIsZoomToggled = false;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = ArmLength;
    SpringArm->SetWorldRotation({-70.f, 0.f, 0.f});
    SpringArm->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APlayerTbsPawn::BeginPlay() {
    Super::BeginPlay();
    // Set initial pose as desired
    InitArmLength = ArmLength;
    DesiredZoom = ArmLength;
    DesiredMoveTarget = GetActorLocation();
    DesiredRotationTarget = GetActorRotation();

    if (APlayerController *PC = Cast<APlayerController>(GetController())) {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                    PC->GetLocalPlayer())) {
            Subsystem->AddMappingContext(CameraMappingContext, 0);
        }
    }
}

// Called every frame
void APlayerTbsPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    FVector NewLocation = FMath::VInterpTo(
        GetActorLocation(), DesiredMoveTarget, DeltaTime, InterpSpeedLocation);
    FRotator NewRotation =
        FMath::RInterpTo(GetActorRotation(), DesiredRotationTarget, DeltaTime,
                         InterpSpeedRotation);
    float NewZoom = FMath::FInterpTo(SpringArm->TargetArmLength, DesiredZoom,
                                     DeltaTime, InterpSpeedZoom);

    SetActorTransform(FTransform(NewRotation, NewLocation));
    SetArmLength(NewZoom);
}

// Called to bind functionality to input
void APlayerTbsPawn::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent *EIComp =
            Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        EIComp->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                           &APlayerTbsPawn::HandleMove);
        EIComp->BindAction(ZoomAction, ETriggerEvent::Triggered, this,
                           &APlayerTbsPawn::HandleZoom);
        EIComp->BindAction(RotateCameraToggle, ETriggerEvent::Started, this,
                           &APlayerTbsPawn::StartRotateToggle);
        EIComp->BindAction(RotateCameraToggle, ETriggerEvent::Completed, this,
                           &APlayerTbsPawn::EndRotateToggle);
        EIComp->BindAction(MouseAxisAction, ETriggerEvent::Triggered, this,
                           &APlayerTbsPawn::HandleMouseAxis);
        EIComp->BindAction(HomeAction, ETriggerEvent::Started, this,
                           &APlayerTbsPawn::HandleHome);
    }
}

// ------------ INPUT HANDLING METHODS
void APlayerTbsPawn::HandleMove(const FInputActionValue &Value) {
    FVector2D InputValue = Value.Get<FVector2D>();
    FVector ActorForward = GetActorForwardVector();
    FVector ActorRight = GetActorRightVector();
    DesiredMoveTarget = {
        DesiredMoveTarget.X +
            InputValue.X * CameraMovementSpeed * ActorForward.X +
            InputValue.Y * CameraMovementSpeed * ActorRight.X,
        DesiredMoveTarget.Y +
            InputValue.X * CameraMovementSpeed * ActorForward.Y +
            InputValue.Y * CameraMovementSpeed * ActorRight.Y,
        DesiredMoveTarget.Z};
}

void APlayerTbsPawn::HandleZoom(const FInputActionValue &Value) {
    float InputValue = Value.Get<float>();
    DesiredZoom -= InputValue * ArmLengthChangeSpeed;
    DesiredZoom = FMath::Clamp(DesiredZoom, MinZoom, MaxZoom);
}

void APlayerTbsPawn::StartRotateToggle() { bIsZoomToggled = true; }

void APlayerTbsPawn::EndRotateToggle() { bIsZoomToggled = false; }

void APlayerTbsPawn::HandleMouseAxis(const FInputActionValue &Value) {
    if (!bIsZoomToggled)
        return;
    FVector2D InputValue = Value.Get<FVector2D>();
    DesiredRotationTarget.Yaw += InputValue.X * CameraRotationSpeed;
}

void APlayerTbsPawn::HandleHome() {
    DesiredRotationTarget.Yaw = 0;
    DesiredZoom = InitArmLength;
}
// ------------ END INPUT HANDLING METHODS

void APlayerTbsPawn::SetArmLength(float NewArmLength) {
    ArmLength = NewArmLength;
    if (SpringArm)
        SpringArm->TargetArmLength = NewArmLength;
}

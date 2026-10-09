// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"

// clang-format off
#include "PlayerTbsPawn.generated.h"
// clang-format on

UCLASS(Config = Game)
class OVERGROWTHTACTICS_API APlayerTbsPawn : public APawn {
    GENERATED_BODY()

  public:
    APlayerTbsPawn();

  protected:
    virtual void BeginPlay() override;

    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent *Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent *SpringArm;

    // Inputs
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext *CameraMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction *ZoomAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction *MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction *RotateCameraToggle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction *MouseAxisAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction *HomeAction;

    // Config properties
    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float ArmLength;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float ArmLengthChangeSpeed;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float CameraMovementSpeed;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float CameraRotationSpeed;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float MinZoom;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float MaxZoom;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float InterpSpeedZoom;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float InterpSpeedLocation;

    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite,
              Category = "Camera Settings")
    float InterpSpeedRotation;

  public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(
        class UInputComponent *PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetArmLength(float NewArmLength);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetArmLengthChangeSpeed(float NewArmLengthChangeSpeed) {
        ArmLengthChangeSpeed = NewArmLengthChangeSpeed;
    }

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraMovementSpeed(float NewCameraMovementSpeed) {
        CameraMovementSpeed = NewCameraMovementSpeed;
    }

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraRotationSpeed(float NewCameraRotationSpeed) {
        CameraRotationSpeed = NewCameraRotationSpeed;
    }

  private:
    float InitArmLength;

    // Desired waypoints
    float DesiredZoom;
    FVector DesiredMoveTarget;
    FRotator DesiredRotationTarget;

    // Rotate toggle
    bool bIsZoomToggled;

    // Camera movement handlers
    void HandleMove(const FInputActionValue &Value);
    void HandleZoom(const FInputActionValue &Value);
    void StartRotateToggle();
    void EndRotateToggle();
    void HandleMouseAxis(const FInputActionValue &Value);
    void HandleHome();
};

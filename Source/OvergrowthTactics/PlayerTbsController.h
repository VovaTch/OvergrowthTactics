// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// clang-format off
#include "PlayerTbsController.generated.h"
// clang-format on

/**
 *
 */
UCLASS()
class OVERGROWTHTACTICS_API APlayerTbsController : public APlayerController {
    GENERATED_BODY()

  public:
    virtual void BeginPlay() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Action.h"
#include "CoreMinimal.h"

// clang-format off
#include "Action_RemoveTile.generated.h"
// clang-format on

/**
 *
 */
UCLASS()
class OVERGROWTHTACTICS_API AAction_RemoveTile : public AAction {
    GENERATED_BODY()

  public:
    virtual void ExecuteAction_Implementation(FIntPoint GridIndex) override;

  protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

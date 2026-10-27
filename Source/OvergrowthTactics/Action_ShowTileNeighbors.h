// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Action.h"
#include "CoreMinimal.h"

// clang-format off
#include "Action_ShowTileNeighbors.generated.h"
// clang-format on

/**
 *
 */
UCLASS()
class OVERGROWTHTACTICS_API AAction_ShowTileNeighbors : public AAction {
    GENERATED_BODY()

  public:
    virtual void ExecuteAction_Implementation(FIntPoint GridIndex) override;

  protected:
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
    void HandleHoveredTileChanged(FIntPoint GridIndex);

  private:
    TArray<FIntPoint> SavedNeighbors = {};

    void ResetPreviousNeighborStates();
};

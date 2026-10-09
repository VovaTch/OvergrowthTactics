// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "OvergrowthTactics/Action.h"
#include "OvergrowthTactics/Grid.h"

// clang-format off
#include "PlayerActions.generated.h"
// clang-format on

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectedActionsChanged);

UCLASS()
class OVERGROWTHTACTICS_API APlayerActions : public AActor {
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    APlayerActions();

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    TObjectPtr<AGrid> Grid;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Actions")
    APlayerController *PlayerController;

    // Inputs
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext *ActionsMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction *LeftClickAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction *RightClickAction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
    AAction *SelectedAction_LeftClick;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
    AAction *SelectedAction_RightClick;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Grid Actions")
    void UpdateTileUnderCursor();

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void SetSelectedAction(TSubclassOf<AAction> LeftActionClass,
                           TSubclassOf<AAction> RightActionClass);

    // Getters
    UFUNCTION(BlueprintCallable, Category = "Grid Actions")
    FIntPoint GetHoveredTileIdx() { return HoveredTileIdx; }

    UFUNCTION(BlueprintCallable, Category = "Grid Actions")
    FIntPoint GetSelectedTileIdx() { return SelectedTileIdx; }

    UFUNCTION(BlueprintPure, Category = "Actions")
    AAction *GetSelectedActionLeft() const { return SelectedAction_LeftClick; }

    UFUNCTION(BlueprintPure, Category = "Actions")
    AAction *GetSelectedActionRight() const {
        return SelectedAction_RightClick;
    }

    TObjectPtr<AGrid> GetGridPtr() { return Grid; }

    // Setters
    UFUNCTION(BlueprintCallable, Category = "Grid Actions")
    void SetSelectedTileIdx(FIntPoint TileIdx) { SelectedTileIdx = TileIdx; }

    // Callbacks
    void OnLeftClickPressed();
    void OnRightClickPressed();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Actions")
    FOnSelectedActionsChanged OnSelectedActionsChanged;

  private:
    FIntPoint HoveredTileIdx{-99999, -99999};
    FIntPoint SelectedTileIdx{-99999, -99999};
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"
#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"
#include "GameFramework/Actor.h"
#include "Grid.h"

// clang-format off
#include "DebugTextOnTiles.generated.h"
// clang-format on

UCLASS()
class OVERGROWTHTACTICS_API ADebugTextOnTiles : public AActor {
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    ADebugTextOnTiles();

    TObjectPtr<ATextRenderActor> GetTextActor(const FIntPoint Index);
    void DestroyTextActor(const FIntPoint Index);

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY()
    TObjectPtr<AGrid> Grid = nullptr;

    UPROPERTY()
    TMap<FIntPoint, TObjectPtr<ATextRenderActor>> TextActors;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bShowTileIndices = false;

    UFUNCTION(BlueprintCallable)
    void SetShowTileIndices(bool ShowTileIndices) {
        bShowTileIndices = ShowTileIndices;
    }

    UFUNCTION(BlueprintCallable)
    void ClearAllTextActors();

    UFUNCTION(BlueprintCallable)
    void UpdateTextOnTile(const FIntPoint Index);

    UFUNCTION(BlueprintCallable)
    void UpdateStateOnAllTiles();

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};

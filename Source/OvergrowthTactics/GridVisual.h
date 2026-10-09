// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridShapeData.h"
#include "OvergrowthTactics/GridMeshInstance.h"

// clang-format off
#include "GridVisual.generated.h"
// clang-format on

UCLASS()
class OVERGROWTHTACTICS_API AGridVisual : public AActor {
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AGridVisual();

    void InitializeGridVisuals(UStaticMesh *Mesh, UMaterialInterface *Material);
    void UpdateTileVisuals(const FTileData &TileData);
    void DestroyGridVisuals();
    void SetOffsetFromGround(float Offset);

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Clean up the spawned mesh-instance actor when this visual is destroyed
    virtual void Destroyed() override;

    UPROPERTY()
    TObjectPtr<AGridMeshInstance> GridMeshInstance;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};

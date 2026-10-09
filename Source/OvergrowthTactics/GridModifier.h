// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OvergrowthTactics/GridShapeData.h"

// clang-format off
#include "GridModifier.generated.h"
// clang-format on

UCLASS()
class OVERGROWTHTACTICS_API AGridModifier : public AActor {
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AGridModifier();
    virtual void OnConstruction(const FTransform &Transform) override;

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *ModifierMesh;

    // Read only data table
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UDataTable> GridShapeData;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Modifier")
    EGridShape GridShape = EGridShape::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Modifier")
    ETileType TileType = ETileType::Obstacle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

// clang-format off
#include "GridShapeData.generated.h"
// clang-format on

class UMaterialInstance;
class UStaticMesh;

UENUM(BlueprintType)
enum class EGridShape : uint8 {
    None UMETA(DisplayName = "None"),
    Square UMETA(DisplayName = "Square"),
    Hexagon UMETA(DisplayName = "Hexagon"),
    Triangle UMETA(DisplayName = "Triangle"),
};

UENUM(BlueprintType)
enum class ETileType : uint8 {
    None UMETA(DisplayName = "none"),
    Normal UMETA(DisplayName = "normal"),
    Obstacle UMETA(DisplayName = "obstacle"),
};

UENUM(BlueprintType)
enum class ETileState : uint8 {
    None UMETA(DisplayName = "none"),
    Hovered UMETA(DisplayName = "hovered"),
    Selected UMETA(DisplayName = "selected"),
};

FORCEINLINE static bool IsTileTypeWalkable(ETileType Type) {
    switch (Type) {
    case ETileType::Normal:
        return true;
    case ETileType::Obstacle:
        return false;
    case ETileType::None:
        return false;
    default:
        return false;
    }
}

USTRUCT(BlueprintType)
struct FTileData {
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint Index = FIntPoint(0, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETileType Type = ETileType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform Transform = FTransform::Identity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ETileState> States;
};

USTRUCT(BlueprintType) struct FGridShapeData : public FTableRowBase {
    GENERATED_BODY();

  public:
    FGridShapeData()
        : MeshSize(FVector(100.f, 100.f, 100.f)), BaseMesh(nullptr),
          BaseMaterial(nullptr), FlatMesh(nullptr), FlatMaterial(nullptr),
          BorderMaterial(nullptr) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Shape Data")
    FVector MeshSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Shape Data")
    TObjectPtr<UStaticMesh> BaseMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Shape Data")
    TObjectPtr<UMaterialInstance> BaseMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Shape Data")
    TObjectPtr<UStaticMesh> FlatMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Shape Data")
    TObjectPtr<UMaterialInstance> FlatMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Shape Data")
    TObjectPtr<UMaterialInstance> BorderMaterial;
};

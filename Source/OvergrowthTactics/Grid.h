// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OvergrowthTactics/GridShapeData.h"
#include "OvergrowthTactics/GridVisual.h"

// clang-format off
#include "Grid.generated.h"
// clang-format on

UCLASS()
class OVERGROWTHTACTICS_API AGrid : public AActor {
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AGrid();

    virtual void OnConstruction(const FTransform &Transform) override;
    void AddGridTile(const FTileData &TileData);
    void RemoveGridTile(const FTileData &TileData);

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid
    // Properties") UInstancedStaticMeshComponent *GridInstanceComponent;

    // Grid properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
    FVector OriginPosition = {0.f, 0.f, 0.f};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid Properties")
    FVector GridBottomLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
    FVector TileSize = {200.f, 200.f, 100.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
    FIntPoint TileCount = {1, 1};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
    EGridShape GridShape = EGridShape::Square;

    // Environment-aware toggle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
    bool bUseEnvironment = true;

    // Read only data table TODO: remove?
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    TObjectPtr<UDataTable> GridShapeData;

    // NEW DATA LAYER
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
    TMap<FIntPoint, FTileData> GridTileMap;

    // NEW VISUAL LAYER
    UPROPERTY()
    TObjectPtr<UChildActorComponent> GridVisualPresenter;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Function to refresh grid, callable from outside C++
    UFUNCTION(BlueprintCallable, Category = "Grid Logic")
    void RefreshGrid();

    UFUNCTION(BlueprintCallable, Category = "Grid Interface")
    FVector GetCursorLocationOnGrid(APlayerController *PlayerController);

    UFUNCTION(BlueprintCallable, Category = "Grid Interface")
    FIntPoint GetTileIndexFromWorldLocation(FVector Location);

    // State handlers
    UFUNCTION(BlueprintCallable, Category = "Grid Interface")
    void AddStateToTile(ETileState State, FIntPoint Index);

    UFUNCTION(BlueprintCallable, Category = "Grid Interface")
    void RemoveStateFromTile(ETileState State, FIntPoint Index);

    UFUNCTION(BlueprintCallable, Category = "Grid Interface")
    void UpdateTileVisuals(const FTileData &TileData);

    // Bools
    UFUNCTION(BlueprintCallable, Category = "Grid Interface")
    bool IsIndexValid(const FIntPoint &Index);

    // Getters
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Grid Interface")
    void GetTileData(FIntPoint Index, FTileData &TileData, bool &Success);

    UFUNCTION(BlueprintCallable, Category = "Grid Interface")
    FVector GetTileScale();

  private:
    // UStaticMesh *SquareAsset;
    // UStaticMesh *HexAsset;
    // UStaticMesh *TriAsset;

    void UpdateGridBottomLeft();
    void GenerateGridSquare();
    void GenerateGridTri();
    void GenerateGridHex();
    void DestroyGrid();

    // Get grid Idx for shapes; inaccurate for Tri and Hex, but probably
    // irrelevant
    FIntPoint GetGridIdxSquare(FVector LocationOnGrid);
    FIntPoint GetGridIdxTri(FVector LocationOnGrid);
    FIntPoint GetGridIdxHex(FVector LocationOnGrid);

    ETileType TraceForGround(const FVector &Start, FVector &End);
    FGridShapeData *GetRowFromShape(EGridShape Shape);
};

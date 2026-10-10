// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/Optional.h"
#include "OvergrowthTactics/GridModifier.h"
#include "OvergrowthTactics/GridShapeData.h"
#include "OvergrowthTactics/GridUtils.h"
#include "OvergrowthTactics/OvergrowthTactics.h"

// Sets default values
AGrid::AGrid() {
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create a root component
    USceneComponent *Root =
        CreateDefaultSubobject<USceneComponent>("RootComponent");
    SetRootComponent(Root);

    GridVisualPresenter = CreateDefaultSubobject<UChildActorComponent>(
        TEXT("GridVisualPresenter"));
    GridVisualPresenter->SetupAttachment(Root);
    GridVisualPresenter->SetChildActorClass(AGridVisual::StaticClass());
}

// Called when the game starts or when spawned
void AGrid::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGrid::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AGrid::OnConstruction(const FTransform &Transform) {
    Super::OnConstruction(Transform);
    OriginPosition = GetActorLocation();
    RefreshGrid();
}

void AGrid::RefreshGrid() {

    DestroyGrid();
    if (!GridShapeData || !GridVisualPresenter)
        return;

    GridTileMap.Empty();

    FGridShapeData *Row = GetRowFromShape(GridShape);
    if (Row) {
        if (!GridVisualPresenter->GetChildActor())
            GridVisualPresenter->CreateChildActor();

        auto VisualPresenter =
            Cast<AGridVisual>(GridVisualPresenter->GetChildActor());
        if (VisualPresenter)
            VisualPresenter->InitializeGridVisuals(Row->FlatMesh,
                                                   Row->BorderMaterial);
    }

    FString EnumStr = StaticEnum<EGridShape>()->GetValueAsString(GridShape);

    switch (GridShape) {
    case EGridShape::Square:
        GenerateGridSquare();
        break;
    case EGridShape::Hexagon:
        GenerateGridHex();
        break;
    case EGridShape::Triangle:
        GenerateGridTri();
        break;
    case EGridShape::None:
        break;
    }
}

TOptional<FTileData> AGrid::GetTileDataFromIndex(const FIntPoint &Index) {

    FGridShapeData *Row = GetRowFromShape(GridShape);
    if (!Row || !Row->FlatMesh)
        return NullOpt;
    FVector MeshBounds = Row->FlatMesh->GetBounds().BoxExtent * 2.f;

    if (MeshBounds.X <= 0.f || MeshBounds.Y <= 0.f)
        return NullOpt;

    FVector InstanceScale = {TileSize.X / MeshBounds.X,
                             TileSize.Y / MeshBounds.Y, 1.f};
    FVector PlacementLocation;

    switch (GridShape) {
    case EGridShape::Square:
        PlacementLocation = {
            GridBottomLeft.X +
                (static_cast<float>(Index.X) + 0.5f) * TileSize.X,
            GridBottomLeft.Y +
                (static_cast<float>(Index.Y) + 0.5f) * TileSize.Y,
            GridBottomLeft.Z + 0.1};
        break;
    case EGridShape::Hexagon:
        PlacementLocation = {
            GridBottomLeft.X +
                (static_cast<float>(Index.X) + 1.f / 3.f) * TileSize.X * 1.5 +
                fmod(Index.Y, 2) * TileSize.X * 0.75,
            GridBottomLeft.Y +
                (static_cast<float>(Index.Y) + 1.f) * TileSize.Y * 0.5,
            GridBottomLeft.Z + 0.1};
    case EGridShape::Triangle:
        PlacementLocation = {
            GridBottomLeft.X +
                (static_cast<float>(Index.X) + 0.5f) * TileSize.X,
            GridBottomLeft.Y +
                (static_cast<float>(Index.Y) + 1.f) * TileSize.Y * 0.5,
            GridBottomLeft.Z + 0.1};
    default:
        return NullOpt;
    }
    ETileType Result = ETileType::Normal; // TODO: is it correct?

    if (bUseEnvironment) {
        FVector EndPlacement = PlacementLocation;
        Result = TraceForGround(PlacementLocation, EndPlacement);
        if (IsTileTypeWalkable(Result)) {
            PlacementLocation = EndPlacement + 0.1;
        } else {
            return NullOpt;
        }
    }

    FTransform Transform;
    Transform.SetLocation(PlacementLocation);
    Transform.SetRotation(FQuat::Identity);
    Transform.SetScale3D(InstanceScale);

    FTileData NewTile;
    NewTile.Index = Index;
    NewTile.Type = Result;
    NewTile.Transform = Transform;

    return NewTile;
}

void AGrid::UpdateGridBottomLeft() {

    float TotalGridSizeX;
    float TotalGridSizeY;
    FVector AddedVec;

    switch (GridShape) {
    case EGridShape::Square:
        TotalGridSizeX = TileSize.X * TileCount.X;
        TotalGridSizeY = TileSize.Y * TileCount.Y;
        AddedVec = {fmod(TileCount.X + 1, 2) * 0.5 * TileSize.X,
                    fmod(TileCount.Y + 1, 2) * 0.5 * TileSize.Y, 0.f};
        GridBottomLeft = {OriginPosition.X - TotalGridSizeX / 2.f,
                          OriginPosition.Y - TotalGridSizeY / 2.f,
                          OriginPosition.Z};
        GridBottomLeft += AddedVec;
        break;

    case EGridShape::Hexagon:
        TotalGridSizeX = TileSize.X * (1.5 * TileCount.X + 0.25);
        TotalGridSizeY = TileSize.Y * (TileCount.Y + 1) * 0.5;
        AddedVec = {fmod(TileCount.X + 1, 2) * TileSize.X / 2.f,
                    fmod(TileCount.Y + 1, 2) * TileSize.Y / 2.f, 0.f};
        GridBottomLeft = {OriginPosition.X - TotalGridSizeX / 2.f,
                          OriginPosition.Y - TotalGridSizeY / 2.f,
                          OriginPosition.Z};
        // GridBottomLeft += AddedVec; TODO: See if correct
        break;

    case EGridShape::Triangle:
        TotalGridSizeX = TileSize.X * TileCount.X;
        TotalGridSizeY = TileSize.Y * (TileCount.Y + 1) * 0.5;
        GridBottomLeft = {OriginPosition.X - TotalGridSizeX / 2.f,
                          OriginPosition.Y - TotalGridSizeY / 2.f,
                          OriginPosition.Z};
        AddedVec = {fmod(TileCount.X + 1, 2) * TileSize.X / 2.f,
                    fmod(TileCount.Y + 1, 2) * TileSize.Y / 2.f, 0.f};
        // GridBottomLeft += AddedVec; TODO: See if correct
        break;
    case EGridShape::None:
        GridBottomLeft = {0, 0, 0};
        break;
    }
}

void AGrid::GenerateGridSquare() {
    FGridShapeData *Row = GetRowFromShape(GridShape);
    if (!Row || !Row->FlatMesh)
        return;

    FVector MeshBounds = Row->FlatMesh->GetBounds().BoxExtent * 2.f;

    if (MeshBounds.X <= 0.f || MeshBounds.Y <= 0.f)
        return;

    FVector InstanceScale = {TileSize.X / MeshBounds.X,
                             TileSize.Y / MeshBounds.Y, 1.f};

    OriginPosition.X = FMath::GridSnap(OriginPosition.X, TileSize.X);
    OriginPosition.Y = FMath::GridSnap(OriginPosition.Y, TileSize.Y);
    OriginPosition.Z = FMath::GridSnap(OriginPosition.Z, TileSize.Z);

    UpdateGridBottomLeft();

    for (auto IdxY = 0; IdxY < TileCount.Y; IdxY++) {
        for (auto IdxX = 0; IdxX < TileCount.X; IdxX++) {

            auto TileData = GetTileDataFromIndex({IdxX, IdxY});

            if (TileData.IsSet()) {
                AddGridTile(TileData.GetValue());
            } else {
                continue;
            }
        }
    }
}

void AGrid::GenerateGridHex() {
    FGridShapeData *Row = GetRowFromShape(GridShape);
    if (!Row || !Row->FlatMesh)
        return;

    //
    FVector MeshBounds = Row->FlatMesh->GetBounds().BoxExtent * 2.f;

    if (MeshBounds.X <= 0.f || MeshBounds.Y <= 0.f)
        return;
    //
    FVector InstanceScale = {TileSize.X / MeshBounds.X,
                             TileSize.Y / MeshBounds.Y, 1.f};

    OriginPosition.X = FMath::GridSnap(OriginPosition.X, TileSize.X * 1.5);
    OriginPosition.Y = FMath::GridSnap(OriginPosition.Y, TileSize.Y);
    OriginPosition.Z = FMath::GridSnap(OriginPosition.Z, TileSize.Z);

    UpdateGridBottomLeft();

    //
    for (auto IdxY = 0; IdxY < TileCount.Y; IdxY++) {
        for (auto IdxX = 0; IdxX < TileCount.X; IdxX++) {

            auto TileData = GetTileDataFromIndex({IdxX, IdxY});

            if (TileData.IsSet()) {
                AddGridTile(TileData.GetValue());
            } else {
                continue;
            }
        }
    }
}

void AGrid::GenerateGridTri() {
    // First destroy the previous grid; I don't think I need very strong
    // performance so it doesn't need to be iterative
    FGridShapeData *Row = GetRowFromShape(GridShape);
    if (!Row || !Row->FlatMesh)
        return;

    FVector MeshBounds = Row->FlatMesh->GetBounds().BoxExtent * 2.f;

    if (MeshBounds.X <= 0.f || MeshBounds.Y <= 0.f)
        return;

    FVector InstanceScale = {TileSize.X / MeshBounds.X,
                             TileSize.Y / MeshBounds.Y, 1.f};

    OriginPosition.X = FMath::GridSnap(OriginPosition.X, TileSize.X * 2);
    OriginPosition.Y = FMath::GridSnap(OriginPosition.Y, TileSize.Y);
    OriginPosition.Z = FMath::GridSnap(OriginPosition.Z, TileSize.Z);

    UpdateGridBottomLeft();

    for (auto IdxY = 0; IdxY < TileCount.Y; IdxY++) {
        for (auto IdxX = 0; IdxX < TileCount.X; IdxX++) {

            auto TileData = GetTileDataFromIndex({IdxX, IdxY});

            if (TileData.IsSet()) {
                AddGridTile(TileData.GetValue());
            } else {
                continue;
            }
        }
    }
}

void AGrid::DestroyGrid() {
    GridTileMap.Empty();
    if (GridVisualPresenter) {
        if (auto VisualPresenter =
                Cast<AGridVisual>(GridVisualPresenter->GetChildActor()))
            VisualPresenter->DestroyGridVisuals();
    }
}

FGridShapeData *AGrid::GetRowFromShape(EGridShape Shape) {
    return GetGridShapeRow(GridShapeData, Shape);
}

ETileType AGrid::TraceForGround(const FVector &Start, FVector &End) {

    // Emergency fallback
    End = Start;

    FVector TraceStart = Start + FVector(0.f, 0.f, 10000.f);
    FVector TraceEnd = Start - FVector(0.f, 0.f, 10000.f);

    float TraceRadius = FMath::Min(TileSize.X, TileSize.Y) * 0.20f;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // Default tile type
    ETileType DetectedType = ETileType::None;

    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults, TraceStart, TraceEnd, FQuat::Identity, COLLISION_GROUND,
        FCollisionShape::MakeSphere(TraceRadius), Params);

    if (bHit && HitResults.Num() > 0) {

        DetectedType = ETileType::Normal;

        for (const FHitResult &HitResult : HitResults) {

            // Snap the tile onto the surface that was actually hit.
            End.X = Start.X;
            End.Y = Start.Y;

            float AdjustedZ = HitResult.ImpactPoint.Z;

            if (TileSize.Z > 0) {
                End.Z = FMath::GridSnap(AdjustedZ, TileSize.Z);
            } else {
                End.Z = AdjustedZ;
            }

            if (AGridModifier *GridModifier =
                    Cast<AGridModifier>(HitResult.GetActor())) {
                DetectedType = GridModifier->TileType;
                break;
            }
        }
    }

    return DetectedType;
}

void AGrid::AddGridTile(const FTileData &TileData) {
    GridTileMap.Add(TileData.Index, TileData);
    if (GridVisualPresenter) {
        auto VisualPresenter =
            Cast<AGridVisual>(GridVisualPresenter->GetChildActor());
        if (VisualPresenter)
            VisualPresenter->UpdateTileVisuals(TileData);
    }
}

void AGrid::RemoveGridTile(const FTileData &TileData) {
    if (!IsIndexValid(TileData.Index))
        return;
    GridTileMap.Remove(TileData.Index);
    if (GridVisualPresenter) {
        auto VisualPresenter =
            Cast<AGridVisual>(GridVisualPresenter->GetChildActor());
        if (VisualPresenter)
            VisualPresenter->UpdateTileVisuals(TileData);
    }
}

FVector AGrid::GetCursorLocationOnGrid(APlayerController *PlayerController) {
    if (!PlayerController)
        return FVector(-99999.f, -99999.f, -99999.f);

    FHitResult HitResult;
    bool bHit = PlayerController->GetHitResultUnderCursorByChannel(
        UEngineTypes::ConvertToTraceType(ECC_Visibility), false, HitResult);

    if (bHit)
        return HitResult.Location;

    FVector WorldLocation;
    FVector WorldDirection;
    if (PlayerController->DeprojectMousePositionToWorld(WorldLocation,
                                                        WorldDirection)) {
        FVector LineEnd = WorldLocation + WorldDirection * 10000.f;
        FVector PlanePoint = GetActorLocation();
        FVector PlaneNormal = FVector(0.f, 0.f, 1.f);
        float T;
        FVector IntersectionPoint;

        if (UKismetMathLibrary::LinePlaneIntersection_OriginNormal(
                WorldLocation, LineEnd, PlanePoint, PlaneNormal, T,
                IntersectionPoint)) {
            return IntersectionPoint;
        }
    }

    return FVector(-99999.f, -99999.f, -99999.f);
}

FIntPoint AGrid::GetTileIndexFromWorldLocation(FVector Location) {
    FVector LocationOnGrid = Location - GridBottomLeft;

    switch (GridShape) {
    case EGridShape::Square:
        return GetGridIdxSquare(LocationOnGrid);
    case EGridShape::Triangle:
        return GetGridIdxTri(LocationOnGrid);
    case EGridShape::Hexagon:
        return GetGridIdxHex(LocationOnGrid);
    case EGridShape::None:
        return FIntPoint(-99999, -99999);
    }

    return FIntPoint(-99999, -99999);
}

// Get grid index functions
FIntPoint AGrid::GetGridIdxSquare(FVector LocationOnGrid) {
    float SnappedX =
        FMath::GridSnap(LocationOnGrid.X + 0.5 * TileSize.X, TileSize.X) -
        0.5 * TileSize.X;
    float SnappedY =
        FMath::GridSnap(LocationOnGrid.Y + 0.5 * TileSize.Y, TileSize.Y) -
        0.5 * TileSize.Y;

    int IdxX = FMath::FloorToInt(SnappedX / TileSize.X);
    int IdxY = FMath::FloorToInt(SnappedY / TileSize.Y);

    return FIntPoint(IdxX, IdxY);
}
FIntPoint AGrid::GetGridIdxTri(FVector LocationOnGrid) {
    int IdxX = FMath::FloorToInt(LocationOnGrid.X / TileSize.X);
    int IdxY = FMath::RoundToInt(LocationOnGrid.Y / (TileSize.Y * 0.5f) - 1.f);

    return FIntPoint(IdxX, IdxY);
}
/**
 * @brief From Manus/Windsurf
 *
 * This function computes the indices given location on grid for a hexagonal
 * grid. Made by Manus, couldn't bother with the math because I'm going to use
 * the square one.
 *
 * @param LocationOnGrid The location on grid relative to the grid origin.
 * @return The x and y indices of the grid
 *
 * */
FIntPoint AGrid::GetGridIdxHex(FVector LocationOnGrid) {
    const float StepX = TileSize.X * 1.5f;
    const float StepY = TileSize.Y * 0.5f;

    int BaseY = FMath::RoundToInt(LocationOnGrid.Y / StepY - 1.f);

    int BestX = 0;
    int BestY = BaseY;
    float BestDistSq = TNumericLimits<float>::Max();

    for (int dy = -1; dy <= 1; ++dy) {
        int IdxY = BaseY + dy;
        float OffsetX =
            static_cast<float>(((IdxY % 2) + 2) % 2) * TileSize.X * 0.75f;
        int IdxX =
            FMath::RoundToInt((LocationOnGrid.X - OffsetX) / StepX - 1.f / 3.f);

        float CenterX =
            (static_cast<float>(IdxX) + 1.f / 3.f) * StepX + OffsetX;
        float CenterY = (static_cast<float>(IdxY) + 1.f) * StepY;

        float DiffX = LocationOnGrid.X - CenterX;
        float DiffY = LocationOnGrid.Y - CenterY;
        float DistSq = DiffX * DiffX + DiffY * DiffY;

        if (DistSq < BestDistSq) {
            BestDistSq = DistSq;
            BestX = IdxX;
            BestY = IdxY;
        }
    }

    return FIntPoint(BestX, BestY);
}

void AGrid::GetTileData(FIntPoint Index, FTileData &TileData, bool &Success) {
    auto Tile = GridTileMap.Find(Index);
    if (!Tile) {
        TileData = {};
        Success = false;
    } else {
        TileData = *Tile;
        Success = true;
    }
}

// ----------------- STATE HANDLERS ---------------
void AGrid::AddStateToTile(ETileState State, FIntPoint Index) {
    auto Tile = GridTileMap.Find(Index);
    if (!Tile)
        return;
    Tile->States.AddUnique(State);
    UpdateTileVisuals(*Tile);
}
void AGrid::RemoveStateFromTile(ETileState State, FIntPoint Index) {
    auto Tile = GridTileMap.Find(Index);
    if (!Tile)
        return;
    if (Tile->States.Contains(State)) {
        Tile->States.Remove(State);
        UpdateTileVisuals(*Tile);
    }
}

void AGrid::UpdateTileVisuals(const FTileData &TileData) {
    if (!GridVisualPresenter)
        return;
    if (auto *Visual = Cast<AGridVisual>(GridVisualPresenter->GetChildActor()))
        Visual->UpdateTileVisuals(TileData);
}

// ---------------- GETTERS
FVector AGrid::GetTileScale() {
    FGridShapeData *Row = GetRowFromShape(GridShape);
    if (!Row || !Row->FlatMesh)
        return {0.0, 0.0, 1.0};

    FVector MeshBounds = Row->FlatMesh->GetBounds().BoxExtent * 2.f;

    if (MeshBounds.X <= 0.f || MeshBounds.Y <= 0.f)
        return {0.0, 0.0, 1.0};
    return {TileSize.X / MeshBounds.X, TileSize.Y / MeshBounds.Y, 1.f};
}

// ---------------- BOOL CHECKS
bool AGrid::IsIndexValid(const FIntPoint &Index) {
    auto Tile = GridTileMap.Find(Index);
    if (Tile) {
        return true;
    } else {
        return false;
    }
}

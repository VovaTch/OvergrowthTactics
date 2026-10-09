// Fill out your copyright notice in the Description page of Project Settings.

#include "GridVisual.h"
#include "Components/SceneComponent.h"
#include "GridMeshInstance.h"
#include "OvergrowthTactics/GridShapeData.h"

// Sets default values
AGridVisual::AGridVisual() {
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    SetActorTickEnabled(false);
}

// Called when the game starts or when spawned
void AGridVisual::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGridVisual::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AGridVisual::InitializeGridVisuals(UStaticMesh *Mesh,
                                        UMaterialInterface *Material) {
    if (!GridMeshInstance) {
        GridMeshInstance =
            GetWorld()->SpawnActor<AGridMeshInstance>(FActorSpawnParameters());
        GridMeshInstance->AttachToActor(
            this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }

    GridMeshInstance->InitMeshInst(Mesh, Material, FLinearColor::Black,
                                   ECollisionEnabled::QueryOnly);
    if (USceneComponent *Root = GetRootComponent())
        Root->SetRelativeLocation(FVector::ZeroVector);
}

void AGridVisual::UpdateTileVisuals(const FTileData &TileData) {
    if (!GridMeshInstance)
        return;
    GridMeshInstance->RemoveGridInst(TileData.Index);

    if (IsTileTypeWalkable(TileData.Type)) {
        GridMeshInstance->AddGridInst(TileData.Index, TileData.Transform);
        GridMeshInstance->UpdateGridInstColor(TileData.Index, TileData.States);
    }
}

void AGridVisual::DestroyGridVisuals() {
    if (GridMeshInstance)
        GridMeshInstance->ClearGridInst();
}

void AGridVisual::Destroyed() {
    if (GridMeshInstance) {
        GridMeshInstance->Destroy();
        GridMeshInstance = nullptr;
    }
    Super::Destroyed();
}

void AGridVisual::SetOffsetFromGround(float Offset) {
    if (USceneComponent *Root = GetRootComponent()) {
        FVector NewLocation = Root->GetRelativeLocation();
        NewLocation.Z += Offset;
        Root->SetRelativeLocation(NewLocation);
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "GridModifier.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "OvergrowthTactics/GridUtils.h"

// Sets default values
AGridModifier::AGridModifier() {
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create a root component
    USceneComponent *Root =
        CreateDefaultSubobject<USceneComponent>("RootComponent");
    SetRootComponent(Root);

    ModifierMesh = CreateDefaultSubobject<UStaticMeshComponent>("ModifierMesh");
    ModifierMesh->SetupAttachment(Root);

    SetActorHiddenInGame(true);
}

void AGridModifier::OnConstruction(const FTransform &Transform) {
    Super::OnConstruction(Transform);
    if (!GridShapeData)
        return;

    auto GridShapeRow = GetGridShapeRow(GridShapeData, GridShape);
    if (!GridShapeRow)
        return;
    UStaticMesh *BlockMesh = GridShapeRow->BaseMesh;
    auto Texture = GridShapeRow->FlatMaterial;
    if (BlockMesh) {
        ModifierMesh->SetStaticMesh(BlockMesh);
        ModifierMesh->SetMaterial(0, Texture);
    }

    FLinearColor TargetColor = FLinearColor::Black;
    switch (TileType) {
    case ETileType::Normal:
        TargetColor = FLinearColor::White;
        break;
    case ETileType::Obstacle:
        TargetColor = FLinearColor::Red;
        break;
    default:
        break;
    }

    UMaterialInstanceDynamic *DynMaterial =
        ModifierMesh->CreateDynamicMaterialInstance(0);
    if (DynMaterial) {
        DynMaterial->SetVectorParameterValue("Color", TargetColor);
    }

    // ModifierMesh->SetVectorParameterValueOnMaterials(TEXT("Color"),
    //                                                  FVector(TargetColor));
    ModifierMesh->SetCollisionObjectType(
        ECollisionChannel::ECC_GameTraceChannel1); // TODO: change to the
                                                   // correct one
    ModifierMesh->SetCollisionResponseToAllChannels(
        ECollisionResponse::ECR_Ignore);
    ModifierMesh->SetCollisionResponseToChannel(
        ECollisionChannel::ECC_GameTraceChannel1,
        ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AGridModifier::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGridModifier::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

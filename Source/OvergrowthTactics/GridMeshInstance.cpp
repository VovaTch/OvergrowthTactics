// Fill out your copyright notice in the Description page of Project Settings.

#include "GridMeshInstance.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "OvergrowthTactics/GridShapeData.h"

// Sets default values
AGridMeshInstance::AGridMeshInstance() {
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    SetActorTickEnabled(false);

    ISMComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
        TEXT("ISMComponent"));
    SetRootComponent(ISMComponent);
}

// Called when the game starts or when spawned
void AGridMeshInstance::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGridMeshInstance::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AGridMeshInstance::InitMeshInst(UStaticMesh *Mesh,
                                     UMaterialInterface *Material,
                                     FLinearColor Color,
                                     ECollisionEnabled::Type Collision) {
    if (!ISMComponent)
        return;

    ISMComponent->SetStaticMesh(Mesh);
    ISMComponent->SetMaterial(0, Material);
    ISMComponent->NumCustomDataFloats = 4;
    UMaterialInstanceDynamic *DynMat =
        ISMComponent->CreateAndSetMaterialInstanceDynamic(0);
    if (DynMat)
        DynMat->SetVectorParameterValue(TEXT("Color"), Color);
    ISMComponent->SetCollisionEnabled(Collision);
}

void AGridMeshInstance::AddGridInst(FIntPoint Index,
                                    const FTransform &Transform) {
    if (!ISMComponent)
        return;

    RemoveGridInst(Index);

    int32 NewISMIndex = ISMComponent->AddInstance(Transform);
    InstanceMapping.Add(Index, NewISMIndex);
}

void AGridMeshInstance::RemoveGridInst(FIntPoint Index) {

    if (!ISMComponent || !InstanceMapping.Contains(Index))
        return;

    int32 ISMIndexToRemove = InstanceMapping[Index];
    if (ISMComponent->RemoveInstance(ISMIndexToRemove)) {
        InstanceMapping.Remove(Index);
        for (auto &Kvp : InstanceMapping) {
            if (Kvp.Value > ISMIndexToRemove) {
                Kvp.Value--;
            }
        }
    }
}

void AGridMeshInstance::ClearGridInst() {
    if (ISMComponent)
        ISMComponent->ClearInstances();
    InstanceMapping.Empty();
}

void AGridMeshInstance::SetColorStates(FVector ColorNone, FVector ColorHovered,
                                       FVector ColorSelected) {
    ColorStates[ETileState::None] = ColorNone;
    ColorStates[ETileState::Hovered] = ColorHovered;
    ColorStates[ETileState::Selected] = ColorSelected;
}

FVector AGridMeshInstance::GetColorFromStates(TArray<ETileState> States) {
    const TArray<ETileState> Heirarchy{ETileState::Selected,
                                       ETileState::Hovered, ETileState::None};
    for (auto TileState : Heirarchy) {
        if (States.Contains(TileState)) {
            if (auto Color = ColorStates.Find(TileState)) {
                return *Color;
            }
        }
    }
    return FVector(0.f, 0.f, 0.f);
}

float AGridMeshInstance::GetOpacityFromStates(TArray<ETileState> States) {
    const TArray<ETileState> Heirarchy{ETileState::Selected,
                                       ETileState::Hovered, ETileState::None};

    for (auto TileState : Heirarchy) {
        if (States.Contains(TileState)) {
            if (auto Opacity = OpacityStates.Find(TileState)) {
                return *Opacity;
            }
        }
    }

    return 0.f;
}

void AGridMeshInstance::UpdateGridInstColor(FIntPoint Index,
                                            const TArray<ETileState> &States) {
    if (!ISMComponent || !InstanceMapping.Contains(Index))
        return;
    const int ISMIndex = InstanceMapping[Index];
    const FVector Color = GetColorFromStates(States);
    const float Opacity = GetOpacityFromStates(States);
    ISMComponent->SetCustomDataValue(ISMIndex, 0, Color.X, false);
    ISMComponent->SetCustomDataValue(ISMIndex, 1, Color.Y, false);
    ISMComponent->SetCustomDataValue(ISMIndex, 2, Color.Z, false);
    ISMComponent->SetCustomDataValue(ISMIndex, 3, Opacity, false);
}

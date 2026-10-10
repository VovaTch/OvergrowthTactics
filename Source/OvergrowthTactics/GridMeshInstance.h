// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OvergrowthTactics/GridShapeData.h"

// clang-format off
#include "GridMeshInstance.generated.h"
// clang-format on

class UInstancedStaticMeshComponent;

UCLASS()
class OVERGROWTHTACTICS_API AGridMeshInstance : public AActor {
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AGridMeshInstance();

    void InitMeshInst(UStaticMesh *Mesh, UMaterialInterface *Material,
                      FLinearColor Color, ECollisionEnabled::Type Collision);
    void AddGridInst(FIntPoint Index, const FTransform &Transform);
    void RemoveGridInst(FIntPoint Index);
    void ClearGridInst();
    void SetColorStates(FVector ColorNone, FVector ColorHovered,
                        FVector ColorSelected);
    FVector GetColorFromStates(TArray<ETileState> States);
    float GetOpacityFromStates(TArray<ETileState> States);
    void UpdateGridInstColor(FIntPoint Index, const TArray<ETileState> &States);

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UInstancedStaticMeshComponent> ISMComponent;

  private:
    UPROPERTY()
    TMap<FIntPoint, int32> InstanceMapping;

    UPROPERTY()
    TMap<ETileState, FVector> ColorStates = {
        {ETileState::None, FVector(0.f, 0.f, 0.f)},
        {ETileState::Selected, FVector(1.f, 0.0f, 0.0f)},
        {ETileState::Hovered, FVector(0.2f, 0.6f, 0.2f)}};

    UPROPERTY()
    TMap<ETileState, float> OpacityStates = {{ETileState::None, 0.f},
                                             {ETileState::Selected, 0.85f},
                                             {ETileState::Hovered, 0.3f}};

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "Action_ShowTileNeighbors.h"
#include "OvergrowthTactics/GridShapeData.h"
#include "PlayerActions.h"

void AAction_ShowTileNeighbors::ExecuteAction_Implementation(
    FIntPoint GridIndex) {

    if (!PlayerActions)
        return;
    auto Grid = PlayerActions->GetGridPtr();
    ResetPreviousNeighborStates();
    if (!Grid || !Grid->IsIndexValid(GridIndex)) {
        return;
    }

    auto Neighbors = Grid->GetNeighbors(GridIndex);
    for (const auto Neighbor : Neighbors) {
        Grid->AddStateToTile(ETileState::IsNeighbor, Neighbor);
        SavedNeighbors.Add(Neighbor);
    }
}

void AAction_ShowTileNeighbors::BeginPlay() {
    Super::BeginPlay();

    PlayerActions = Cast<APlayerActions>(GetOwner());
    if (PlayerActions) {
        PlayerActions->OnHoveredTileChanged.AddDynamic(
            this, &AAction_ShowTileNeighbors::HandleHoveredTileChanged);
    }
}

void AAction_ShowTileNeighbors::EndPlay(
    const EEndPlayReason::Type EndPlayReason) {

    PlayerActions = Cast<APlayerActions>(GetOwner());
    if (PlayerActions) {
        PlayerActions->OnHoveredTileChanged.RemoveDynamic(
            this, &AAction_ShowTileNeighbors::HandleHoveredTileChanged);
    }
    ResetPreviousNeighborStates();
    Super::EndPlay(EndPlayReason);
}

void AAction_ShowTileNeighbors::ResetPreviousNeighborStates() {

    if (!PlayerActions)
        return;
    auto Grid = PlayerActions->GetGridPtr();
    if (!Grid)
        return;

    for (const auto Neighbor : SavedNeighbors) {
        Grid->RemoveStateFromTile(ETileState::IsNeighbor, Neighbor);
    }

    SavedNeighbors.Empty();
}

void AAction_ShowTileNeighbors::HandleHoveredTileChanged(FIntPoint GridIndex) {
    ExecuteAction(GridIndex);
}

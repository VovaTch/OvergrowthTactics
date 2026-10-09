// Fill out your copyright notice in the Description page of Project Settings.

#include "Action_SelectTile.h"
#include "OvergrowthTactics/GridShapeData.h"
#include "PlayerActions.h"

void AAction_SelectTile::ExecuteAction_Implementation(FIntPoint GridIndex) {
    if (!PlayerActions)
        return;
    auto Grid = PlayerActions->GetGridPtr();
    if (!Grid) {
        PlayerActions->SetSelectedTileIdx(FIntPoint{-99999, -99999});
        return;
    }

    FIntPoint CurrentlySelected = PlayerActions->GetSelectedTileIdx();
    if (GridIndex != CurrentlySelected) {

        Grid->RemoveStateFromTile(ETileState::Selected, CurrentlySelected);
        PlayerActions->SetSelectedTileIdx(GridIndex);
        Grid->AddStateToTile(ETileState::Selected, GridIndex);
    } else {
        Grid->RemoveStateFromTile(ETileState::Selected, CurrentlySelected);
        PlayerActions->SetSelectedTileIdx(FIntPoint{-99999, -99999});
    }
}

void AAction_SelectTile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    ExecuteAction_Implementation(FIntPoint{-99999, -99999});
    Super::EndPlay(EndPlayReason);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Action_RemoveTile.h"
#include "OvergrowthTactics/Action.h"
#include "PlayerActions.h"

void AAction_RemoveTile::ExecuteAction_Implementation(FIntPoint GridIndex) {
    if (!PlayerActions)
        return;
    auto Grid = PlayerActions->GetGridPtr();
    if (!Grid->IsIndexValid(GridIndex)) {
        return;
    }

    auto TileData = Grid->GetTileDataFromIndex(GridIndex);
    if (TileData.IsSet()) {
        Grid->RemoveGridTile(TileData.GetValue());
    }
}

void AAction_RemoveTile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
}

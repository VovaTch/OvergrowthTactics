// Fill out your copyright notice in the Description page of Project Settings.

#include "Action_AddTile.h"
#include "OvergrowthTactics/GridShapeData.h"
#include "PlayerActions.h"

void AAction_AddTile::ExecuteAction_Implementation(FIntPoint GridIndex) {
    if (!PlayerActions)
        return;
    auto Grid = PlayerActions->GetGridPtr();
    if (!Grid->IsIndexValid(GridIndex)) {
        return;
    }

    FTileData TileData;
    bool Success = false;

    Grid->GetTileData(GridIndex, TileData, Success);
}

void AAction_AddTile::EndPlay(const EEndPlayReason::Type EndPlayReason) {}

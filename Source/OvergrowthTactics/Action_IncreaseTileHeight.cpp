// Fill out your copyright notice in the Description page of Project Settings.

#include "Action_IncreaseTileHeight.h"
#include "PlayerActions.h"

void AAction_IncreaseTileHeight::ExecuteAction_Implementation(
    FIntPoint GridIndex) {
    if (!PlayerActions)
        return;
    auto Grid = PlayerActions->GetGridPtr();
    if (!Grid->IsIndexValid(GridIndex)) {
        return;
    }

    FTileData TileData;
    bool ExtractionSuccess;
    Grid->GetTileData(GridIndex, TileData, ExtractionSuccess);
    if (!ExtractionSuccess)
        return;

    TileData.Transform.SetLocation(TileData.Transform.GetLocation() +
                                   FVector(0, 0, Grid->GetTileSize().Z));

    Grid->AddGridTile(TileData);
}

void AAction_IncreaseTileHeight::EndPlay(
    const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Action_SelectTileType.h"
#include "PlayerActions.h"

void AAction_SelectTileType::ExecuteAction_Implementation(FIntPoint GridIndex) {

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

    TileData.Type = TileType;
    Grid->AddGridTile(TileData);
}

void AAction_SelectTileType::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
}

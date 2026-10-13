// Fill out your copyright notice in the Description page of Project Settings.

#include "Action_DecreaseTileHeight.h"
#include "PlayerActions.h"

void AAction_DecreaseTileHeight::ExecuteAction_Implementation(
    FIntPoint GridIndex) {

    if (!PlayerActions)
        return;
    auto Grid = PlayerActions->GetGridPtr();
    if (!Grid->IsIndexValid(GridIndex)) {
        return;
    }

    auto TileData = Grid->GetTileDataFromIndex(GridIndex);
    if (!TileData.IsSet())
        return;

    TileData->Transform.SetLocation(TileData->Transform.GetLocation() +
                                    FVector(0, 0, -Grid->GetTileScale().Z));
    ;
}

void AAction_DecreaseTileHeight::EndPlay(
    const EEndPlayReason::Type EndPlayReason) {}

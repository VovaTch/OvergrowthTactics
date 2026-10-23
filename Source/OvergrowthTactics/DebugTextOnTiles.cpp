// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugTextOnTiles.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "OvergrowthTactics/GridShapeData.h"

// Sets default values
ADebugTextOnTiles::ADebugTextOnTiles() {
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADebugTextOnTiles::BeginPlay() {

    Super::BeginPlay();
    auto FoundGrid =
        UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass());
    if (FoundGrid) {
        Grid = Cast<AGrid>(FoundGrid);
    }

    if (!Grid)
        return;
    Grid->OnTileDataUpdated.AddDynamic(this,
                                       &ADebugTextOnTiles::UpdateTextOnTile);
    Grid->OnGridDestroyed.AddDynamic(this,
                                     &ADebugTextOnTiles::ClearAllTextActors);
    UpdateStateOnAllTiles();
}

TObjectPtr<ATextRenderActor>
ADebugTextOnTiles::GetTextActor(const FIntPoint Index) {

    if (!Grid)
        return nullptr;

    if (auto TextActor = TextActors.Find(Index)) {
        if (IsValid(*TextActor))
            return *TextActor;
        TextActors.Remove(Index);
    }

    if (!Grid->IsIndexValid(Index))
        return nullptr;

    FTileData TileData;
    bool bExtractionSuccess = false;
    Grid->GetTileData(Index, TileData, bExtractionSuccess);
    if (!bExtractionSuccess)
        return nullptr;

    auto Location = TileData.Transform.GetLocation();
    auto Rotation = FRotationMatrix::Identity.Rotator();
    Rotation.Pitch = 90.0f;
    Rotation.Yaw = 180.0f;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // Do I need this?

    UWorld *World = GetWorld();
    if (!World)
        return nullptr;
    ATextRenderActor *NewTextActor = World->SpawnActor<ATextRenderActor>(
        ATextRenderActor::StaticClass(), Location, Rotation, SpawnParams);

    if (!NewTextActor)
        return nullptr;

    NewTextActor->SetActorEnableCollision(false);
    UTextRenderComponent *TextRenderComponent = NewTextActor->GetTextRender();

    TextRenderComponent->SetText(
        FText::FromString(FString::Printf(TEXT("%d, %d"), Index.X, Index.Y)));
    TextRenderComponent->SetHorizontalAlignment(
        EHorizTextAligment::EHTA_Center);
    TextRenderComponent->SetVerticalAlignment(
        EVerticalTextAligment::EVRTA_TextCenter);
    TextRenderComponent->SetWorldSize(Grid->GetTileSize().X / 2.5);
    TextRenderComponent->SetTextRenderColor(FColor::White);

    if (IsValid(NewTextActor)) {
        TextActors.Add(Index, NewTextActor);
        return NewTextActor;
    } else {
        return nullptr;
    }
}

// Called every frame
void ADebugTextOnTiles::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Tile updates
void ADebugTextOnTiles::DestroyTextActor(const FIntPoint Index) {
    auto TextActor = TextActors.Find(Index);
    if (!TextActor)
        return;
    TextActor->Get()->Destroy();
    TextActors.Remove(Index);
}

void ADebugTextOnTiles::ClearAllTextActors() {
    for (const auto &Kvp : TextActors) {
        if (IsValid(Kvp.Value))
            Kvp.Value->Destroy();
    }
    TextActors.Empty();
}

void ADebugTextOnTiles::UpdateTextOnTile(const FIntPoint Index) {
    if (!Grid)
        return;

    if (!bShowTileIndices) {
        DestroyTextActor(Index);
        return;
    }

    FTileData TileData;
    bool bExtractionSuccess = false;
    Grid->GetTileData(Index, TileData, bExtractionSuccess);

    if (!bExtractionSuccess || !IsTileTypeWalkable(TileData.Type)) {
        DestroyTextActor(Index);
        return;
    }

    auto TextActor = GetTextActor(Index);
    if (!TextActor)
        return;

    auto NewText =
        FText::FromString(FString::Printf(TEXT("%d, %d"), Index.X, Index.Y));

    auto Location = TileData.Transform.GetLocation();
    auto Rotation = FRotationMatrix::Identity.Rotator();
    Rotation.Pitch = 90.0f;
    Rotation.Yaw = 180.0f;

    TextActor->SetActorLocation(Location + FVector(0.0f, 0.0f, 1.0f));
    UTextRenderComponent *TextRenderComponent = TextActor->GetTextRender();
    TextRenderComponent->SetText(NewText);
    TextRenderComponent->SetHorizontalAlignment(
        EHorizTextAligment::EHTA_Center);
    TextRenderComponent->SetVerticalAlignment(
        EVerticalTextAligment::EVRTA_TextCenter);
    TextRenderComponent->SetWorldSize(Grid->GetTileSize().X / 2.5);
    TextRenderComponent->SetTextRenderColor(FColor::White);
}

void ADebugTextOnTiles::UpdateStateOnAllTiles() {
    for (const auto &Kvp : Grid->GetGridTileMap()) {
        UpdateTextOnTile(Kvp.Key);
    }
}

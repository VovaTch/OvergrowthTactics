// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelector.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelSelector::ALevelSelector() { PrimaryActorTick.bCanEverTick = false; }

// Called when the game starts or when spawned
void ALevelSelector::BeginPlay() {
    Super::BeginPlay();
    SelectLevelByString(LoadedLevel, true);
}

// Called every frame
void ALevelSelector::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ALevelSelector::SelectLevelByString(FString LevelName, bool bForce) {

    // Prevent reloading the same level unless forced
    if (!bForce && LevelName.Equals(LoadedLevel, ESearchCase::IgnoreCase))
        return;

    // Unload current level
    if (!LoadedLevel.IsEmpty() &&
        !LoadedLevel.Equals("None", ESearchCase::IgnoreCase)) {
        ULevelStreaming *OldLevelStreaming =
            UGameplayStatics::GetStreamingLevel(this, FName(*LoadedLevel));
        if (OldLevelStreaming) {
            OldLevelStreaming->SetShouldBeVisible(false);
            OldLevelStreaming->SetShouldBeLoaded(false);
        }
    }
    LoadedLevel = LevelName;

    // Return if level name is None; cannot build a None level in UE5.
    if (LoadedLevel.Equals("None", ESearchCase::IgnoreCase))
        return;

    // Load new level
    ULevelStreaming *NewLevelStreaming =
        UGameplayStatics::GetStreamingLevel(this, FName(*LoadedLevel));
    if (NewLevelStreaming) {
        NewLevelStreaming->SetShouldBeLoaded(true);
        NewLevelStreaming->SetShouldBeVisible(true);
    } else {
        UE_LOG(LogTemp, Error, TEXT("Level %s not found"), *LoadedLevel);
    }
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

//clang-format off
#include "LevelSelector.generated.h"
//clang-format on

UCLASS()
class OVERGROWTHTACTICS_API ALevelSelector : public AActor {
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    ALevelSelector();

    UFUNCTION(BlueprintCallable, Category = "Level Selector")
    void SelectLevelByString(FString LevelName, bool bForce = false);

    UFUNCTION(BlueprintPure, Category = "Level Selector")
    FString GetLoadedLevel() const { return LoadedLevel; }

  protected:
    FString LoadedLevel = TEXT("square1");

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};

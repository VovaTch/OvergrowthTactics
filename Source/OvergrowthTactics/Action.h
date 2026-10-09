// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// clang-format off
#include "Action.generated.h"
// clang-format on

class APlayerActions;

UCLASS(Abstract, Blueprintable)
class OVERGROWTHTACTICS_API AAction : public AActor {
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AAction();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action",
              meta = (ExposeOnSpawn = "true"))
    APlayerActions *PlayerActions;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
    void ExecuteAction(FIntPoint GridIndex);
    virtual void ExecuteAction_Implementation(FIntPoint GridIndex);

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};

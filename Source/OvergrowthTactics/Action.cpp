// Fill out your copyright notice in the Description page of Project Settings.

#include "Action.h"

// Sets default values
AAction::AAction() { PrimaryActorTick.bCanEverTick = false; }

// Called when the game starts or when spawned
void AAction::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AAction::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AAction::ExecuteAction_Implementation(FIntPoint GridIndex) {}

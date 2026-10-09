// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerActions.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "OvergrowthTactics/Action_SelectTile.h"
#include "OvergrowthTactics/Grid.h"
#include "OvergrowthTactics/GridShapeData.h"

// Sets default values
APlayerActions::APlayerActions() {
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerActions::BeginPlay() {
    Super::BeginPlay();

    auto FoundGrid =
        UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass());
    if (FoundGrid) {
        Grid = Cast<AGrid>(FoundGrid);
    }

    SetSelectedAction(AAction_SelectTile::StaticClass(), nullptr);

    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController) {
        EnableInput(PlayerController);

        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                    PlayerController->GetLocalPlayer())) {
            if (ActionsMappingContext)
                Subsystem->AddMappingContext(ActionsMappingContext, 0);
        }

        if (UEnhancedInputComponent *EIComp =
                Cast<UEnhancedInputComponent>(InputComponent)) {
            if (LeftClickAction)
                EIComp->BindAction(LeftClickAction, ETriggerEvent::Started,
                                   this, &APlayerActions::OnLeftClickPressed);
            if (RightClickAction)
                EIComp->BindAction(RightClickAction, ETriggerEvent::Started,
                                   this, &APlayerActions::OnRightClickPressed);
        }
    }
}

// Called every frame
void APlayerActions::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    UpdateTileUnderCursor();
}

void APlayerActions::UpdateTileUnderCursor() {
    if (!Grid || !PlayerController)
        return;

    auto CursorLocation = Grid->GetCursorLocationOnGrid(PlayerController);
    auto CurrentTileIdx = Grid->GetTileIndexFromWorldLocation(CursorLocation);

    if (HoveredTileIdx != CurrentTileIdx) {
        Grid->RemoveStateFromTile(ETileState::Hovered, HoveredTileIdx);
        Grid->AddStateToTile(ETileState::Hovered, CurrentTileIdx);
        HoveredTileIdx = CurrentTileIdx;
    }
}

// Callbacks
void APlayerActions::OnLeftClickPressed() {
    if (SelectedAction_LeftClick)
        SelectedAction_LeftClick->ExecuteAction(HoveredTileIdx);
}

void APlayerActions::OnRightClickPressed() {
    if (SelectedAction_RightClick)
        SelectedAction_RightClick->ExecuteAction(HoveredTileIdx);
}

void APlayerActions::SetSelectedAction(TSubclassOf<AAction> LeftActionClass,
                                       TSubclassOf<AAction> RightActionClass) {
    // Click once left
    if (IsValid(SelectedAction_LeftClick)) {
        SelectedAction_LeftClick->Destroy();
        SelectedAction_LeftClick = nullptr;
    }
    if (IsValid(LeftActionClass)) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        SelectedAction_LeftClick = GetWorld()->SpawnActor<AAction>(
            LeftActionClass, GetActorTransform(), SpawnParams);
        if (SelectedAction_LeftClick)
            SelectedAction_LeftClick->PlayerActions = this;
    }

    // Click one right
    if (IsValid(SelectedAction_RightClick)) {
        SelectedAction_RightClick->Destroy();
        SelectedAction_RightClick = nullptr;
    }
    if (IsValid(RightActionClass)) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        SelectedAction_RightClick = GetWorld()->SpawnActor<AAction>(
            RightActionClass, GetActorTransform(), SpawnParams);
        if (SelectedAction_RightClick)
            SelectedAction_RightClick->PlayerActions = this;
    }

    OnSelectedActionsChanged.Broadcast();
}

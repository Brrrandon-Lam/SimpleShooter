// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // Spawn and add widget to viewport
    HUDWidget = CreateWidget(this, HUDClass);
    if(HUDWidget) {
        HUDWidget->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    if(bIsWinner) {
        UUserWidget* WinScreenWidget = CreateWidget(this, WinScreenClass);
        if(WinScreenWidget) {
            WinScreenWidget->AddToViewport();
        }
    }
    else {
        UUserWidget* LoseScreenWidget = CreateWidget(this, LoseScreenClass);
        if(LoseScreenWidget) {
            LoseScreenWidget->AddToViewport();
        }
    }
    HUDWidget->RemoveFromParent();
    // Restart the game on a 5 second delay.
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

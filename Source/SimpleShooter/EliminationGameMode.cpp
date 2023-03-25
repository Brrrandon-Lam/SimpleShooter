// Fill out your copyright notice in the Description page of Project Settings.


#include "EliminationGameMode.h"
#include "EngineUtils.h" // Brings Helpers into scope (TActorRange)
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AEliminationGameMode::PawnKilled(APawn *PawnKilled)
{
    Super::PawnKilled(PawnKilled);
    UE_LOG(LogTemp, Warning, TEXT("Pawn Killed"));
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerController) {
        // Game ended, lost.
        EndGame(false);
    }
    // For loop over ShooterAIControllers in World
    for(AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld())) {
        // If there is a case where a controller is not dead, return from this function
        if(!Controller->IsDead()) {
            return;
        }
    }
    // All ShooterAIControllers are dead, so we need to display our win screen.
    EndGame(true);
        // Is not dead?
        // If one is not dead, early return from this function
    // If we finish looping through without an early return, then EndGame(true)
}

void AEliminationGameMode::EndGame(bool bIsPlayerWinner)
{
    // Range based for loop, for every controller pointer in the range of controllers in the world.
    for(AController* Controller : TActorRange<AController>(GetWorld())) {
        // Returns whether or not the current actor is the winner.
        bool bIsWinner = (Controller->IsPlayerController() == bIsPlayerWinner);
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
    // bIsWinner will be true if:
        // It is a player controller and it is a winner
         // If it is not a player controller and the player is not the winner
    // bIsWinner will be false if:
        // It is a player controller and not the winner
        // It is not a player controller but it is the winner.
    // This will result in correct behavior
    // Generally we can evaluate all of the cases with booleans in a single line.
    // If the player has won, it will try to take away the focus from the player pawn, so nullptr will cause focus issues.
    // In a lot of match-style games, we may want to follow the pawn that was the winner.
    // In our game, we don't care about that, we will continue to look at ourselves whether we win or lose.
}

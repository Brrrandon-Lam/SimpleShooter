// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();
    if(AIBehaviorTree) {
        RunBehaviorTree(AIBehaviorTree);
        //APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        //GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PlayerLocation")), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(FName(TEXT("StartingLocation")), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

// We want to pass this on to ShooterCharacter
bool AShooterAIController::IsDead() const
{
    // Get the pawn associated with our controller and attempt to cast it to our ShooterCharacter class.
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    // If the cast was successful
    if(ControlledCharacter) {
        // Return the value of GetIsDead().
        return ControlledCharacter->GetIsDead();
    }
    // If we're not controlling a pawn, our AI controller might as well be dead.
    // The pawn disconnects from an AI controller when it does.
    return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
    NodeName = "Update Last Known Location";
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // Super
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    // Get the player pawn from UGameplayStatics
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    // Get the player's location using the pawn
    if(!PlayerPawn)
    {
        return;
    }
    // We want to do the behavior we had in Tick in AIController.cpp
    // We want to get the blackboard component and set the player's last known location
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"



// Constructor
UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = "Update Player Location";
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    // First, get the player pawn from UGameplayStatics.
    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(!Player) {
        return;
    }
    // Store the player's location in a vector
    //FVector PlayerLocation = Player->GetActorLocation();
    if(!OwnerComp.GetAIOwner()) {
        return;
    }
    // Then get the AI Controller from OwnerComp.
    // If line of sight, set the player object blackboard key
    if(OwnerComp.GetAIOwner()->LineOfSightTo(Player, FVector::ZeroVector)) {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Player);
    }
    // Otherwise clear the blackboard key's value.
    else {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}

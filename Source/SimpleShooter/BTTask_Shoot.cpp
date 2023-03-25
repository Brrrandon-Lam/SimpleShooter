// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "ShooterCharacter.h"
#include "AIController.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    // Get the pawn (ShooterCharacter)
    // We can get hold of the AI controller.
    // From the AI controller we can get a pawn
    // We then need to cast the pawn to a ShooterCharacter.
    // Call the shoot method from the ShooterCharacter class.
    // Check that we grabbed the AI owner
    if(!OwnerComp.GetAIOwner()) {
        return EBTNodeResult::Failed;
    }
    // Return success. We don't need this task to run contiuously. Just for a single frame.
    AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    // Check that the cast was successful. Don't need to check GetPawn() because it is implicit.
    if(!Character) {
        return EBTNodeResult::Failed;
    }
    Character->Shoot();
    return EBTNodeResult::Succeeded;
}
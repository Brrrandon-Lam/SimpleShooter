// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "EliminationGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AEliminationGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PawnKilled(APawn* PawnKilled) override;
	void EndGame(bool bIsPlayerWinner);
	
};

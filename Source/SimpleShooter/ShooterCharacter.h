// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

// Forward declare this class to keep our compile-time down.
class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();
	// Returns a boolean to determine if the actor is dead.
	UFUNCTION(BlueprintPure)
	bool GetIsDead() const; 
	// Returns the % health remaining of the character
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Override from Actor class. Will be used to receive damage.
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	// Calls PullTrigger
	void Shoot();

private:
	void MoveForward(float AxisValue);
	void MoveLR(float AxisValue);
	UPROPERTY(EditAnywhere) 
	float RotationRate = 20.0;

	void LookVerticalGamepad(float AxisValue);
	void LookHorizontalGamepad(float AxisValue);

	// Don't want to be able to edit this at runtime.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;
	// Tell blueprint to restrict the classes we can select from to be exclusively of the Gun C++ class
	// Not a pointer, not pointing to an instance of the gun, rather we are getting a class itself.

	// Don't want to be able to edit this at all.
	// This is the actual gun. Once we've created an instance of the class this is where we'll store it.
	UPROPERTY()
 	AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth = 100.0;
};

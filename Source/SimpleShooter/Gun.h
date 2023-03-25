// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	// Pulls the trigger on the gun.
	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// A visible anywhere pointer to the root component.
	UPROPERTY(VisibleAnywhere)
	USceneComponent *Root;

	// A visible anywhere pointer to the mesh (skeletal, not static)
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* Muzzle;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 50;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	// Refactoring our ray trace
	bool GunTrace(FHitResult &HitResult, FVector &ShotDirection);

	AController* GetOwnerController() const;
};

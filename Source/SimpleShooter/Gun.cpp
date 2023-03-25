// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController *AGun::GetOwnerController() const
{
    // Get the owner of the gun
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) {
		return nullptr;
	}
	// Return the controller of the pawn from OwnerPawn
	return OwnerPawn->GetController();
}

bool AGun::GunTrace(FHitResult &HitResult, FVector &ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if(!OwnerController) {
		return false;
	}
	// Set Actors to ignore in our line trace.
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	// Get the player's viewport location and rotation
	FVector PlayerLocation;
	FRotator PlayerRotation;
	OwnerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	// Calculate the end point of our line trace
	FVector EndPoint = PlayerLocation + PlayerRotation.Vector() * MaxRange;
	// Set our second OutParameter variable: the shot direction.
	ShotDirection = PlayerRotation.Vector() * -1;
	// Modify our HitResult OutParameter and return true or false (whether or not the trace succeeded)
	return GetWorld()->LineTraceSingleByChannel(HitResult, PlayerLocation, EndPoint, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

// Placeholder for what happens when the trigger gets pulled.
void AGun::PullTrigger() 
{
	// Spawn muzzle flash emitter
	UGameplayStatics::SpawnEmitterAttached(Muzzle, Mesh, TEXT("MuzzleFlashSocket"));
	// Play a sound at the barrel of our gun
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;
	bool bHit = GunTrace(HitResult, ShotDirection);
	if(bHit) {
		// Spawn a particle at the impact point
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.ImpactPoint, ShotDirection.Rotation());
		// Play a sound at the impact point
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);

		// Handle damage taken if an actor was hit
		AActor* HitActor = HitResult.GetActor();
		if(HitActor) {
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}

	}
}
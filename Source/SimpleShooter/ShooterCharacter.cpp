// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Set the gun of this character to a spawned gun object defined by GunClass.
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	// Get the mesh and hide weapon_r by name, not doing anything with EPhysBodyOp.
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	// Attaches our object to a component. Takes in what Scene Component it is to be attached to, the transform rules, and the name of the thing.
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_Socket"));
	// This ownership comes into play in multiplayer and damage calculation. The gun will be able to get references to the owning character later.
	// This is for completion.
	Gun->SetOwner(this);

	CurrentHealth = MaxHealth;
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Pitch
	PlayerInputComponent->BindAxis(TEXT("LookVertical"), this, &APawn::AddControllerPitchInput);
	// Bind Yaw
	PlayerInputComponent->BindAxis(TEXT("LookHorizontal"), this, &APawn::AddControllerYawInput);
	// Bind Forward/Backwards
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	// Bind Left/Right
	PlayerInputComponent->BindAxis(TEXT("MoveLR"), this, &AShooterCharacter::MoveLR);
	// Bind Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	// Bind Gamepad Pitch
	PlayerInputComponent->BindAxis(TEXT("LookVerticalGamepad"), this, &AShooterCharacter::LookVerticalGamepad);
	// Bind Gamepad Yaw
	PlayerInputComponent->BindAxis(TEXT("LookHorizontalGamepad"), this, &AShooterCharacter::LookHorizontalGamepad);
	
	// Bind the shoot button for the player
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
	// TakeDamage returns a float, the amount of damage applied. We hold this in a float.
	// If the current health is less than the amount of damage we take, subtract it such that it is exactly zero. (Not Necessary).
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(CurrentHealth, DamageApplied);
	CurrentHealth -= DamageApplied;
	UE_LOG(LogTemp, Warning, TEXT("Actor health remaining: %f"), CurrentHealth);

	if(GetIsDead()) {
		// Send to the game mode that the pawn was killed
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode) {
			GameMode->PawnKilled(this);
		}
		// Disable collision on the capsule component
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Detach controller from the pawn
		DetachFromControllerPendingDestroy();
	}
	// Returns the damage that was applied
	return DamageApplied;
}

// Adds a movement input, multiplying the forward vector by the axis value.
void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveLR(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookVerticalGamepad(float AxisValue)
{
	// If we multiply axis value by rotation rate we get a speed.
	// To get from speed to distance, multiply by time (delta seconds)
	// This is the amount of time we need to apply this movement over.
	// Get the amount of time that has ellapsed in the last frame.
	//AddMovementInput() does the exact same thing, just under the hood.
	// AddMovementInput() assumes it's a rate already, so we don't have to handle our input as a function of time
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookHorizontalGamepad(float AxisValue)
{
	// Multiply the axis value by the rotation rate by time to get distance.
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

/* Redundant
void AShooterCharacter::LookUp(float AxisValue)
{
	APawn::AddControllerPitchInput(AxisValue);
}
*/

void AShooterCharacter::Shoot() {
	Gun->PullTrigger();
}

bool AShooterCharacter::GetIsDead() const {
	return CurrentHealth <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	// Returns the percentage of our max health that we have left.
	return CurrentHealth / MaxHealth;
}
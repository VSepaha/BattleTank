// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay() {
	Super::BeginPlay();
	LastFireTime = GetWorld()->TimeSeconds;
}


void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (RoundsLeft <= 0) {
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((GetWorld()->TimeSeconds - LastFireTime) < ReloadTimeInSeconds) {
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving()) {
		FiringState = EFiringState::Aiming;
	}
	else {
		FiringState = EFiringState::Locked;
	}
		
}

EFiringState UTankAimingComponent::GetFiringState() const {
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const {
	return RoundsLeft;
}


void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet) {
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

bool UTankAimingComponent::IsBarrelMoving() {

	if (!ensure(Barrel)) return false;

	FVector BarrelForward = Barrel->GetForwardVector().GetSafeNormal();
	return !BarrelForward.Equals(AimDirection, 0.01);
};

void UTankAimingComponent::AimAt(FVector HitLocation) {
	if (!ensure(Barrel)) { return; }

	FVector OutLaunchVelocity;

	// If it cannot find projectile, it will just return the location of the barrel
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// Calculate the OUTLaunchVelocity
	bool AimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace //Parameter must be present to remove bug
	);

	if (AimSolution) {
		// Turns it into a unit vector
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards();
	}

}

void UTankAimingComponent::MoveBarrelTowards() {

	if (!ensure(Barrel && Turret)) return;

	// Workout difference between current barrel rotation and aim direction
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) <  180) {
		Turret->RotateTurret(DeltaRotator.Yaw);
	}
	else {
		Turret->RotateTurret(-DeltaRotator.Yaw);
	}
	
}

void UTankAimingComponent::Fire() {

	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming) {
		if (!ensure(Barrel)) return;
		if (!ensure(ProjectileBlueprint)) return;

		//Spawn a projectile at the location
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = GetWorld()->TimeSeconds;
		RoundsLeft--;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"
// Depends on movement component via pathfinding

void ATankAIController::BeginPlay() {
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!ensure(PlayerTank)) return;

	// Move Towards player
	MoveToActor(PlayerTank, AcceptanceRadius);

	// Aim Towards the player
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	// Fire if ready
	// TODO fix firing
	AimingComponent->Fire(); // TODO limit firing rate
	
}
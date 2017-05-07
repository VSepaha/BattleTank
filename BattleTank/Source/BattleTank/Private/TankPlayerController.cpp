// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();

	FString PlayerControllerName = GetControlledTank()->GetName();
	if (GetControlledTank() != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController: %s"), *PlayerControllerName);
	} else {
		UE_LOG(LogTemp, Error, TEXT("No PlayerController Found!"));
	}
}

void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimTowardsCrossHair();
	
}

ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrossHair() {
	if (!GetControlledTank()) return; // If no tank then there is no point of this function
	
	FVector OutHitLocation; // OUT parameter
	if (GetSightRayHitLocation(OutHitLocation)) { // It is going to line trace (ray cast)
		// Tell the control tank to aim at this point
		GetControlledTank()->AimAt(OutHitLocation);
	}

}

// Get World location of line trace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector &OutHitLocation) const {
	// Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ScreenLocation = FVector2D(ViewportSizeX*CrossHairXLocation, 
											ViewportSizeY*CrossHairYLocation);

	// "De-project" the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		// Line-trace along that look direction, and see what we hit (max range)
		GetLookVectorHitLocation(LookDirection, OutHitLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Look Direction: %s"), *LookDirection.ToString());
	}

	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector &LookDirection) const{
	
	FVector CameraWorldLocation; //To be discarded
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X, 
		ScreenLocation.Y, 
		CameraWorldLocation, 
		LookDirection
	);

}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector &OutHitLocation) const {
	FHitResult HitResult;
	// To get the start location get the player camera manager, then get the camera location
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	// To get the end location we do start location + the direction we are looking in and multiply it by the range
	FVector EndLocation = StartLocation + LookDirection*LineTraceRange;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility)) {
		OutHitLocation = HitResult.Location; // Set the hit location
		return true;
	}
	OutHitLocation = FVector(0);
	return false;
}
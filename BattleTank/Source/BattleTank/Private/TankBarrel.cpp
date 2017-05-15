// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"


void UTankBarrel::Elevate(float RelativeSpeed) {

	// Move the barrel the right amount this frame
	// Given a max elevation speed, and the frame time

	// Relative speed will range from -1 to 1
	RelativeSpeed = FMath::Clamp(RelativeSpeed, -1.f, 1.f);

	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds; //DeltaTime will make it frame rate independent
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;

	auto NewElevation = FMath::Clamp(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	SetRelativeRotation(FRotator(NewElevation, 0.f, 0.f));

}


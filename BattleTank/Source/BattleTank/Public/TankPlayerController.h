// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" // This has to be the last include

class UTankAimingComponent;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	//Dont forget the virtual keyword, otherwise it won't work for some reason
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn* InPawn) override;
	
	// Start the tank moving the barrel so that a shot would hit where the 
	// crosshair intersects the world
	void AimTowardsCrossHair();

	// Return an out parameter, true if hit landscape
	bool GetSightRayHitLocation(FVector &OutHitLocation) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector &OutHitLocation) const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector &LookDirection) const;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.33333;

	// The measuring unit in Unreal is cm, so if we want 10km 
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0;

	UFUNCTION()
	void OnPossessedTankDeath();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingCompRef);

};

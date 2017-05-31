// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay() {
	Super::BeginPlay();
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	
	// Round the incoming damage to nearest int
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);

	// Clamp the amount of damage possible not to exceed current health
	int32 DamageToApply = FMath::Clamp<float>(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("%s is dead"), *GetName())
	}

	UE_LOG(LogTemp, Warning, TEXT("DamageAmount = %f, DamageToApply = %d"), DamageAmount, DamageToApply)
	
	return DamageToApply;
}

float ATank::GetHealthPercentage() const {
	return (float)CurrentHealth / (float)StartingHealth;
}
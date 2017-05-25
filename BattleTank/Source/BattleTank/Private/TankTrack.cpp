// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankTrack.h"


UTankTrack::UTankTrack() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay() {
	Super::BeginPlay();

	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0;
}

void UTankTrack::ApplySidewaysForce() {

	// Calculate the slippage speed
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// Work out the required acceleration this frame to correct
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector(); // a = s/t with direction

	// Calcuate and apply sideways force (F = ma)
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()); // This will get the tank
	FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle) {
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

void UTankTrack::DriveTrack() {
	FVector AppliedForce = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();

	// Get the root component
	// tank component is a scene component that we will cast as a primitive scene component
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(AppliedForce, ForceLocation);
}


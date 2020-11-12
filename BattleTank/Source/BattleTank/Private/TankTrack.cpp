// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


UTankTrack::UTankTrack()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

void UTankTrack::BeginPlay()
{
    Super::BeginPlay();
    OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Drive the tracks
    DriveTrack();
    // apply sideways force
    ApplySidewaysForce();
    // reset throttle
    CurrentThrottle = 0;
}

void UTankTrack::ApplySidewaysForce()
{
    // Calculate the slippage speed
    auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

    // work out the required acceleration this frame to correct
    auto DeltaTime = GetWorld()->GetDeltaSeconds();
    auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector(); // minus at the beginning for opposite acceleration

    // calculate and apply sideways for (F = m a)
    auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // there are two tracks
    TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
    CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

void UTankTrack::DriveTrack() 
{
    auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
    auto ForceLocation = GetComponentLocation();
    auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

}
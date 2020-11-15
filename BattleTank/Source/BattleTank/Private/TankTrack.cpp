// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"


UTankTrack::UTankTrack()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

//void UTankTrack::BeginPlay()
//{
//    Super::BeginPlay();
//    OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
//}

//void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//    // Drive the tracks
//    //DriveTrack();
//    // apply sideways force
//    ApplySidewaysForce();
//    // reset throttle
//    CurrentThrottle = 0;
//}

//void UTankTrack::ApplySidewaysForce()
//{
//    // Calculate the slippage speed
//    auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
//
//    // work out the required acceleration this frame to correct
//    auto DeltaTime = GetWorld()->GetDeltaSeconds();
//    auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector(); // minus at the beginning for opposite acceleration
//
//    // calculate and apply sideways for (F = m a)
//    auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
//    auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // there are two tracks
//    TankRoot->AddForce(CorrectionForce);
//}

void UTankTrack::SetThrottle(float Throttle)
{
    float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
    DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle) 
{
    //auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
    auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
    //auto ForceLocation = GetComponentLocation();
    //auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    //TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
    auto Wheels = GetWheels();
    auto ForcePerWheel = ForceApplied / Wheels.Num();
    for (ASprungWheel* Wheel : Wheels)
    {
        Wheel->AddDrivingForce(ForcePerWheel);
    }

}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
    TArray<ASprungWheel*> ResultArray;
    TArray<USceneComponent*> Children;
    GetChildrenComponents(true, Children);
    for (USceneComponent* Child : Children)
    {
        auto SpawnPointChild = Cast<USpawnPoint>(Child);
        if (!SpawnPointChild) { continue; }

        AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
        auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
        if (!SprungWheel) { continue; }

        ResultArray.Add(SprungWheel);
    }
    return ResultArray;

}
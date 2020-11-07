// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet) {
    LeftTrack = LeftTrackToSet;
    RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw) {
    //UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);

    if (!LeftTrack || !RightTrack) { return; }
    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);
    // TODO prevent double-speed due to dual control use
}


void UTankMovementComponent::IntendTurnRight(float Throw) {
    UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);

    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(-Throw);
    // TODO prevent double-speed due to dual control use
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) {
    // no need to call Super to inherit as we are replacing this function
    //auto OurTankName = GetOwner()->GetName();
    auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
    auto AIForwardIntention = MoveVelocity.GetSafeNormal();

    auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
    IntendMoveForward(ForwardThrow);

    auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
    IntendTurnRight(RightThrow);

    //UE_LOG(LogTemp, Warning, TEXT("%s vectoring to: %s"), *OurTankName, *MoveVelocityString);
}

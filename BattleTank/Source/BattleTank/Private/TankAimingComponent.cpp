// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
    Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
    auto OurTankName = GetOwner()->GetName();
    auto BarrelLocation = Barrel->GetComponentLocation().ToString();
    // UE_LOG(LogTemp, Warning, TEXT("%s aiming at direction: %s from: %s"), *(OurTankName), *(HitLocation.ToString()), *(BarrelLocation));
    // UE_LOG(LogTemp, Warning, TEXT("Firing at speed: %f"), LaunchSpeed);

    FVector OutLaunchVelocity(0);
    FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

    // Calculate the OutLaunchVelocity
    bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this,
                                                                        OutLaunchVelocity,
                                                                        StartLocation,
                                                                        HitLocation,
                                                                        LaunchSpeed,
                                                                        false, 0, 0,
                                                                        ESuggestProjVelocityTraceOption::DoNotTrace);
    /* debug version:
    const TArray<AActor*> ActorsToIgnore;
    bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this,
                                                                        OutLaunchVelocity,
                                                                        StartLocation,
                                                                        HitLocation,
                                                                        LaunchSpeed,
                                                                        false, 0, 0,
                                                                        ESuggestProjVelocityTraceOption::DoNotTrace,
                                                                        FCollisionResponseParams::DefaultResponseParam,
                                                                        ActorsToIgnore,
                                                                        true);
    */
    if(bHaveAimSolution&&(HitLocation!=FVector(0)))
    {
        auto AimDirection = OutLaunchVelocity.GetSafeNormal();
        // UE_LOG(LogTemp, Warning, TEXT("Aiming at : %s"), *(AimDirection.ToString()));
        MoveBarrelTowards(AimDirection);
        auto Time = GetWorld()->GetTimeSeconds();
        // UE_LOG(LogTemp, Warning, TEXT("%f: Aim solution found"), Time);
        UE_LOG(LogTemp, Warning, TEXT("%f: Aiming at : %s"), Time, *(AimDirection.ToString()));
    }
    else
    {
        auto Time = GetWorld()->GetTimeSeconds();
        UE_LOG(LogTemp, Warning, TEXT("%f: No aim solution found"), Time);
    }
    
    
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
    // Work out difference between current barrel rotation and AimDirecdtion
    auto BarrelRotator = Barrel->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - BarrelRotator;
    // UE_LOG(LogTemp, Warning, TEXT("AimAsARotator at : %s"), *(AimAsRotator.ToString()));

    Barrel->Elevate(5); // TODO remove magic number
}
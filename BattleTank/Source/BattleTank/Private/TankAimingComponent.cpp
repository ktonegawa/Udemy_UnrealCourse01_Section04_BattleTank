// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay()
{
    Super::BeginPlay();
    // so that first fire is after initial reload
    LastFireTime = FPlatformTime::Seconds();
    /*if (FiringState == EFiringState::Reloading)
    {
        UE_LOG(LogTemp, Warning, TEXT("Firing State: Reloading"));
    }
    else if (FiringState == EFiringState::Aiming)
    {
        UE_LOG(LogTemp, Warning, TEXT("Firing State: Aiming"));
    }
    else if (FiringState == EFiringState::Locked)
    {
        UE_LOG(LogTemp, Warning, TEXT("Firing State: Locked"));
    }*/
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
    Barrel = BarrelToSet;
    Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    
    //UE_LOG(LogTemp, Warning, TEXT("Is this ticking?"));
    if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Setting to Reloading"));
        FiringState = EFiringState::Reloading;
    }
    else if (IsBarrelMoving())
    {
        //UE_LOG(LogTemp, Warning, TEXT("Setting to Aiming"));
        FiringState = EFiringState::Aiming;
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("Setting to Locked"));
        FiringState = EFiringState::Locked;
    }
    // TODO handle aiming in locked state
}

EFiringState UTankAimingComponent::GetFiringState() const
{
    return FiringState;
}


bool UTankAimingComponent::IsBarrelMoving()
{
    if (!ensure(Barrel)) { return false; }
    auto BarrelForward = Barrel->GetForwardVector();
    return !BarrelForward.Equals(AimDirection, 0.01);
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
    auto OurTankName = GetOwner()->GetName();
    auto BarrelLocation = Barrel->GetComponentLocation().ToString();

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
        AimDirection = OutLaunchVelocity.GetSafeNormal();
        MoveBarrelTowards(AimDirection);
        auto Time = GetWorld()->GetTimeSeconds();
    }
    else
    {
        auto Time = GetWorld()->GetTimeSeconds();
    }
    
    
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
    if (!ensure(Barrel) || !ensure(Turret)) { return; }
    // Work out difference between current barrel rotation and AimDirection
    auto BarrelRotator = Barrel->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - BarrelRotator;

    // always yaw the shortest way
    Barrel->Elevate(DeltaRotator.Pitch);
    if (DeltaRotator.Yaw < 180)
    {
        Turret->Rotate(DeltaRotator.Yaw);
    }
    else 
    {
        Turret->Rotate(-DeltaRotator.Yaw);
    }
}

void UTankAimingComponent::Fire()
{
    //bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
    if (FiringState != EFiringState::Reloading)
    //if (isReloaded)
    {
        // Spawn a projectile at the socket location on the barrel
        if (!ensure(Barrel)) { return; }
        if (!ensure(ProjectileBlueprint)) { return; }
        auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint,
            Barrel->GetSocketLocation(FName("Projectile")),
            Barrel->GetSocketRotation(FName("Projectile")));

        Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
    }


}


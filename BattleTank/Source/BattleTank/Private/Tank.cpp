// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
//#include "TankMovementComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // no need to protect ponter as added construction
    //TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
    //TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement Component"));

    auto TankName = GetName();
    //UE_LOG(LogTemp, Warning, TEXT("%s DONKEY: Tank C++ Construct"), *TankName)
}

void ATank::BeginPlay()
{
    Super::BeginPlay(); // Needed for BP Begin Play to run

    auto TankName = GetName();
    //UE_LOG(LogTemp, Warning, TEXT("%s DONKEY: Tank C++ Begin Play"), *TankName)

    TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
}

void ATank::AimAt(FVector HitLocation)
{
    if (!ensure(TankAimingComponent)) { return; }
    TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::Fire()
{
    auto Time = GetWorld()->GetTimeSeconds();
    //UE_LOG(LogTemp, Warning, TEXT("%f: Tank fires"), Time);

    if (!ensure(Barrel)) { return; }
    bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
    if (isReloaded) 
    { 
        // Spawn a projectile at the socket location on the barrel
        auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint,
            Barrel->GetSocketLocation(FName("Projectile")),
            Barrel->GetSocketRotation(FName("Projectile")));

        Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
    }

    
}
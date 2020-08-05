// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::SetBarrelReference(UStaticMeshComponent* BarrelToSet)
{
    Barrel = BarrelToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
    auto OurTankName = GetOwner()->GetName();
    auto BarrelLocation = Barrel->GetComponentLocation().ToString();
    UE_LOG(LogTemp, Warning, TEXT("%s aiming at direction: %s from: %s"), *(OurTankName), *(HitLocation.ToString()), *(BarrelLocation));
    UE_LOG(LogTemp, Warning, TEXT("Firing at speed: %f"), LaunchSpeed);

    FVector OutLaunchVelocity(0);
    FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

    // Calculate the OutLaunchVelocity
    if (UGameplayStatics::SuggestProjectileVelocity
                            (
                                this, 
                                OutLaunchVelocity, 
                                StartLocation, 
                                HitLocation, 
                                LaunchSpeed, 
                                false, 
                                0, 
                                0, 
                                ESuggestProjVelocityTraceOption::DoNotTrace
                            )
        )
    {
        auto AimDirection = OutLaunchVelocity.GetSafeNormal();
        UE_LOG(LogTemp, Warning, TEXT("Aiming at : %s"), *(AimDirection.ToString()));
    }
    
    
    
}


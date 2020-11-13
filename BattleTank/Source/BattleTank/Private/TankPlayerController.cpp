// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h" // so we can impliment OnDeath

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) { return; }
    FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);
    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);
        if (!ensure(PossessedTank)) { return; }

        // subscribe our local method to the tank's death event
        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
    }
}

void ATankPlayerController::OnPossessedTankDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("Rceieved death"));
    StartSpectatingOnly();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
    if (!GetPawn()) { return; } // e.g. if not posessing
    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!ensure(AimingComponent)) { return; }
    
	FVector OutHitLocation; // Out Parameter
    bool bGotHitLocation = GetSightRayHitLocation(OutHitLocation);
	if (bGotHitLocation) // Has "side-effect", is going to line trace
	{
        AimingComponent->AimAt(OutHitLocation);
	}

}

// Get world location if linetrace through crosshair, true if hits location
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	OutHitLocation = FVector(1.0);
	// Find crosshair position
	int32 ViewPortSizeX, ViewPortSizeY;
	GetViewportSize(ViewPortSizeX, ViewPortSizeY);
	auto ScreenLocation = FVector2D(ViewPortSizeX * CrosshairXLocation, ViewPortSizeY * CrosshairYLocation);
	
	// de-project screen position of the crosshair to world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// Line-trace along that LookDirection and see what we hit
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}
		
	return false;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera))
	{
		HitLocation = HitResult.Location;
		return true;
	}
    HitLocation = FVector(0);
	return false; // line trace didn't succeed
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // To be discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	/*auto PlayerTank = GetControlledTank();
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController cant find player tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController found player: %s"), *(PlayerTank->GetName()));
	}

	UE_LOG(LogTemp, Warning, TEXT("AIController BeginPlay"));*/
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
    auto ControlledTank = Cast<ATank>(GetPawn());

    if (PlayerTank)
    {
        // MOve towards player
        MoveToActor(PlayerTank, AcceptanceRadius); // TODO check if radius is in cm

        // aim towards the player
        //GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
        ControlledTank->AimAt(PlayerTank->GetActorLocation());

        // TODO fire if ready
        //GetControlledTank()->Fire();
        ControlledTank->Fire(); // TODO limit firing
    }
    // UE_LOG(LogTemp, Warning, TEXT("Player controller ticking"));
}

//ATank* ATankAIController::GetControlledTank() const
//{
//	return Cast<ATank>(GetPawn());
//}


//ATank* ATankAIController::GetPlayerTank() const
//{
//	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
//	if (!PlayerPawn) { return nullptr; }
//	return Cast<ATank>(PlayerPawn);
//}



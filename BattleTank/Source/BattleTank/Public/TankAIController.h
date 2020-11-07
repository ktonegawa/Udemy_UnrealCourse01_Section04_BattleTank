// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "AIController.h"
#include "TankAIController.generated.h"

// forward declaration to get rid of the Tank.h from this files header as a way of 
// cleaner compilation
class ATank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
private:
    //ATank* GetControlledTank() const;

    //ATank* GetPlayerTank() const;

    virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

    // how close can the AI tank get
    float AcceptanceRadius = 3000;
	
};

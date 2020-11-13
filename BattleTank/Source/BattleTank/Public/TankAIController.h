// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
    // how close can the AI tank get
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    float AcceptanceRadius = 5000;

private:

    virtual void Tick(float DeltaTime) override;

    virtual void SetPawn(APawn* InPawn) override;

	virtual void BeginPlay() override;

    UFUNCTION()
    void OnPossessedTankDeath();
};

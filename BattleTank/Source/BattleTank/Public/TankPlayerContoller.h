// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerContoller.generated.h" // must be the last include

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerContoller : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATank* GetControlledTank() const;
	
	
};

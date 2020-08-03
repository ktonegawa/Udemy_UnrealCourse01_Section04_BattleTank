// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerContoller.h"


ATank* ATankPlayerContoller::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}


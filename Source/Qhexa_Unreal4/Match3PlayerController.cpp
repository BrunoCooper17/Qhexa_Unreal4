// Fill out your copyright notice in the Description page of Project Settings.

#include "Qhexa_Unreal4.h"
#include "Match3PlayerController.h"

AMatch3PlayerController::AMatch3PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    // We want the mouse cursor to show immediately on startup, without having to click in the window.
    bShowMouseCursor = true;

    // We need click/touch events to interact with our tiles.
    bEnableTouchEvents = bEnableClickEvents = true;
    bEnableTouchOverEvents = bEnableMouseOverEvents = true;

    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("CONSTRUCT PLAYER CONTROLLER"));
}

void AMatch3PlayerController::AddScore(int32 Points, bool bForceInmediateUpdate) {

}

int32 AMatch3PlayerController::GetScore()
{
	return int32();
}

int32 AMatch3PlayerController::GetDisplayedScore()
{
	return int32();
}

void AMatch3PlayerController::TickScoreDisplay()
{
}

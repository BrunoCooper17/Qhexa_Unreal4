// Fill out your copyright notice in the Description page of Project Settings.

#include "Qhexa_Unreal4.h"
#include "Qhexa_Unreal4GameModeBase.h"
#include "Match3PlayerController.h"

AQhexa_Unreal4GameModeBase::AQhexa_Unreal4GameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass =  APawn::StaticClass();
	PlayerControllerClass = AMatch3PlayerController::StaticClass();
}

void AQhexa_Unreal4GameModeBase::BeginPlay() {

}

void AQhexa_Unreal4GameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWigetClass) {
}

void AQhexa_Unreal4GameModeBase::GameRestart() {
}

void AQhexa_Unreal4GameModeBase::GameOver() {
}

bool AQhexa_Unreal4GameModeBase::IsGameActive() {
	return false;
}

void AQhexa_Unreal4GameModeBase::PauseGameTimer() {
}

FString AQhexa_Unreal4GameModeBase::GetRemainingTimeAsString() {
	return FString();
}

FString AQhexa_Unreal4GameModeBase::GetRemainingMovesAsString() {
	return FString();
}

void AQhexa_Unreal4GameModeBase::AddScore(int32 Points) {
}

void AQhexa_Unreal4GameModeBase::setComboPower(int32 NewComboPower) {
}

int32 AQhexa_Unreal4GameModeBase::GetComboPower() {
	return int32();
}

int32 AQhexa_Unreal4GameModeBase::GetMaxComboPower() {
	return int32();
}

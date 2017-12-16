// Fill out your copyright notice in the Description page of Project Settings.

#include "Qhexa_Unreal4.h"
#include "Match3BlueprintFunctionLibrary.h"

APlayerController* UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(UObject* WorldContextObject) {
	return nullptr;
}

bool UMatch3BlueprintFunctionLibrary::IsGameActive(UObject * WorldContextObject) {
	return false;
}

void UMatch3BlueprintFunctionLibrary::PauseGameTimer(UObject * WorldContextObject, bool bPause) {
}

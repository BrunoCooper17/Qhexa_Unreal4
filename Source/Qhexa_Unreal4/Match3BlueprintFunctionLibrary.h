// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Match3BlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class QHEXA_UNREAL4_API UMatch3BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Match3 Gameplay")
		static APlayerController* GetLocalPlayerController(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Match3 Gameplay")
		static bool IsGameActive(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Match3 Gameplay")
		static void PauseGameTimer(UObject* WorldContextObject, bool bPause);
};

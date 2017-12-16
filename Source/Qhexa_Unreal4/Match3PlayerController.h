// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Match3PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class QHEXA_UNREAL4_API AMatch3PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMatch3PlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Game")
		void AddScore(int32 Points, bool bForceInmediateUpdate = false);

	UFUNCTION(BlueprintCallable, Category = "Game")
		int32 GetScore();

	UFUNCTION(BlueprintCallable, Category = "Game")
		int32 GetDisplayedScore();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
		int32 ComboPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
		int32 MaxComboPower;

protected:
	UPROPERTY()
		int32 Score;

	UPROPERTY()
		float DisplayedSocre;

	UPROPERTY(EditAnywhere)
		float ScoreChangeRate;

	void TickScoreDisplay();
	FTimerHandle TickScoreDisplayHandle;
};

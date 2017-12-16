// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Runtime/UMG/Public/UMG.h"
//#include "Runtime/UMG/Public/UMGStyle.h"
//#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
//#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Qhexa_Unreal4GameModeBase.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCubeMatch3Reward {
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timeAwarded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 movesAwarded;
};

UCLASS()
class QHEXA_UNREAL4_API AQhexa_Unreal4GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AQhexa_Unreal4GameModeBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	/* Change the Menu Widget */
	UFUNCTION(BlueprintCallable, Category = "Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWigetClass);
	
	/* How fast the tile are moving in the screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		float TileMoveSpeed;

	/* Funtion that is call when starting a new match3 game */
	UFUNCTION(BlueprintCallable, Category = "Game")
		void GameRestart();

	/* Function to call when the game ends */
	UFUNCTION(BlueprintCallable, Category = "Game")
		void GameOver();

	/* Determine if the game is being played */
	UFUNCTION(BlueprintCallable, Category = "Game")
		bool IsGameActive();

	/* Get remininig game time */
	UFUNCTION(BlueprintCallable, Category = "Game")
		void PauseGameTimer();

	/* List of rewards that are given time to time during the game */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
		TArray<FCubeMatch3Reward> Rewards;

	/* Get Remaining time of the game */
	UFUNCTION(BlueprintCallable, Category = "Game")
		FString GetRemainingTimeAsString();

	/* Get Remaining moves of the game */
	UFUNCTION(BlueprintCallable, Category = "Game")
		FString GetRemainingMovesAsString();

	/* Notifice the player when the player has recieve a bonus reward */
	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
		void AwardBonus();

	/* Score is handled by this class */
	void AddScore(int32 Points);

	/* The game mode understands the concept of combo power */
	void setComboPower(int32 NewComboPower);

	UFUNCTION(BlueprintPure, Category = "Game")
		int32 GetComboPower();

	UFUNCTION(BlueprintPure, Category = "Game")
		int32 GetMaxComboPower();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
		TSubclassOf<UUserWidget> DefeatWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere)
		float TimeRemaining;

	UPROPERTY(EditAnywhere)
		float MovesRemaining;

	FTimerHandle GameOverTimer;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "Match3SaveGame.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMatch3HighScore {
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Points;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bScoreByPlayer;
};

USTRUCT(BlueprintType)
struct FMatch3LevelSaveData {
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FMatch3HighScore GoldScore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FMatch3HighScore SilverScore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FMatch3HighScore BronzeScore;
};

UCLASS()
class QHEXA_UNREAL4_API UMatch3SaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TMap<FString, FMatch3LevelSaveData> Match3SaveData;
};

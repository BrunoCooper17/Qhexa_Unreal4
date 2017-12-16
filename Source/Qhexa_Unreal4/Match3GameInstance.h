// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Match3GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class QHEXA_UNREAL4_API UMatch3GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	bool FindSaveDataForLevel(UObject* WorldContextObject);

	void SaveGame();

	void UpdateSave(UObject* WorldContextObject);

	UPROPERTY()
		class UMatch3SaveGame* InstanceGameData;

protected:

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CubeTile.generated.h"

UENUM(BlueprintType)
namespace ECubeTileState {
	enum Type{
		E_Normal,
		E_Failling,
		E_PendingDelete
	};
}

USTRUCT(BlueprintType)
struct FCubeTileAbilities {
	GENERATED_USTRUCT_BODY();

	bool CanRotate() { return !bPreventRotate; }

protected:
	/* THIS PROPERTY IS USED TO MAKE THE CUBE BE ABLE OR NOT TO ROTATE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint32 bPreventRotate : 1;

public:

};

UCLASS()
class QHEXA_UNREAL4_API ACubeTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	// MY FUNCTIONS
	UFUNCTION()
		void TileSelected(AActor* sender, FKey key);

	UFUNCTION()
		void TileSelectedByTouch(ETouchIndex::Type FingerIndex, AActor* sender);

	UFUNCTION(BlueprintImplementableEvent, Category = "Special Game Event")
		void PlaySelectionEffect(bool bTurnEffectOn);
	
	/*
	UFUNCTION(BlueprintNativeEvent, Category = "Special Game Event")
		void OnMatched();
	virtual void OnMatched_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Special Game Event")
		void OnSwapMove(bool bMoveWillSucced);
	virtual void OnSwapMove_Implentation(bool bMoveWillSucced);
	*/

    UFUNCTION(BlueprintImplementableEvent, Category = "Special Game Event")
        void RotateHorizontal(bool Right);

    UFUNCTION(BlueprintImplementableEvent, Category = "Special Game Event")
        void RotateVertical(bool Up);
	
	void StartFalling(bool bUseCurrentWorldLocation = false);

	UFUNCTION()
		void TickFalling();

	void FinishFalling();

	void SetGridAddress(int32 newLocation);
	int32 GetGridAddress() const;

	UPROPERTY(EditAnywhere, Category = "Cube")
		int32 TileTypeID;

	UPROPERTY(EditAnywhere, Category = "Cube")
		TEnumAsByte<ECubeTileState::Type> CubeTileState;

	UPROPERTY(EditAnywhere, Category = "Cube")
		FCubeTileAbilities Abilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube")
        UStaticMesh* CubeMesh_Model;

protected:
	UPROPERTY()
		UStaticMeshComponent* CubeMeshComponent;
};

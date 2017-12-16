// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CubeTile.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType)
struct FCubeTileType {
	GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ACubeTile> CubeTileToSpaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCubeTileAbilities Abilities;

	FCubeTileType() {
		probability = 1;
	}
};

UCLASS()
class QHEXA_UNREAL4_API AGrid : public AActor
{
	GENERATED_BODY()

    FVector CameraLocation;
    float Distance;
    const float OFFSET = 7.05f;
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	// MY FUNCTIONS
	UPROPERTY(VisibleAnywhere)
		TArray<ACubeTile*> GameCubeTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCubeTileType> CubeTileLibrary;

	// TYPE OF TILE TO SPAWN
	UPROPERTY(EditAnywhere, Category = "Tile")
		TSubclassOf<class ACubeTile> CubeTileToSpaw;

	// SIZE OF A SPACE ON THE GRID (DOES NOT INCLUDE THE BORDERS OR SPACING BETWEEN THE TILES)
	UPROPERTY(EditAnywhere, Category = "Tile")
		FVector2D CubeTileSize;

	// MINIMUM OF MATCHING TILE IN A ROW NEEDED TO SCORE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		int32 MinimumRunLenght;

	// THE WIDTH OF THE GRID (NEEDED TO CALCULATE TILE POSITIONS AND NEIGHBORS)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		int32 GridWidth;

	// THE HEIGHT OF THE GRID (NEEDED TO CALCULATE TILE POSITIONS AND NEIGHBORS)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		int32 GridHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
        float InitAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
        FVector2D CameraDistance;

	ACubeTile* CreateCubeTile(class AActor* TileCube, FVector SpawnLocation, int32 SpawnLocationOnWorld);
	int32 SelectCubeTileFromLibrary();

	ACubeTile* GetCubeTileFromGridAddress(int32 GridAddress);

	UFUNCTION(BlueprintImplementableEvent, meta = (ExpandEnumAsExecs = "MoveTypes"), Category = "Tile")
		void OnMoveMade();

	UFUNCTION(BlueprintCallable, CAtegory = "Tile")
		FVector GetLocationFromGridAddres(int32 GridAddress);
	FVector GetLocationFromGridAddresWhitOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles);

	UFUNCTION(BlueprintCallable, CAtegory = "Tile")
		bool GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset);
	bool AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB);
	void OnCubeTileFinishedFalling(ACubeTile* Tile, int32 LandingGridAddress);
	void OnCubeTileFinishedMatching(ACubeTile* InTile);
	void OnCubeTileFinishedRotating(ACubeTile* InTile);

	void RespawnCubeTiles();
	void RotateCube(ACubeTile* CubeTile);
	bool IsMoveLegal(ACubeTile* CubeTile);

	TArray<ACubeTile*> FindNeighbors(ACubeTile* StartingCubeTile, bool bMustMatchID = true, int32 RunLenght = 3);
	TArray<ACubeTile*> FindCubeTilesOfType(int32 TileTypeID);
	void ExecuteMatch(TArray<ACubeTile*> MatchingTiles);
	void OnTileWasSelected(ACubeTile* NewSelectedTile);

	ACubeTile* ACurrentWorkingCubeTile;

private:
	TArray<ACubeTile*> LastLegalMatch;
	TArray<ACubeTile*> FallingCubeTiles;
	TArray<ACubeTile*> RotatingCubeTiles;
	TArray<ACubeTile*> CubeTileToCheck;
	TArray<ACubeTile*> CubeTilesBeingDestroyed;
	uint32 bPendingRotateMove : 1;
	uint32 bPendingRotateMoveSuccess : 1;

    TArray<ACubeTile*> CubeArray;
};

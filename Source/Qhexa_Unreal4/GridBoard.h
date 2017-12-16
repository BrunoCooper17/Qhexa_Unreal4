// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.h"
#include "GridBoard.generated.h"

USTRUCT(BlueprintType)
struct FTileType {
    GENERATED_USTRUCT_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ATile> TileToSpaw;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float probability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTileAbilities Abilities;

    FTileType() {
        probability = 1;
    }
};

USTRUCT(BlueprintType)
struct FMatchOrientacion {
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHorizontalMatch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVerticalMatch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDiagonalMatch;

	FMatchOrientacion() {
		bHorizontalMatch = true;
		bVerticalMatch = true;
		bDiagonalMatch = true;
	}
};

USTRUCT(BlueprintType)
struct FMatchCaras {
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCaraSuperiorMatch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCaraFrontalMatch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllEqual;

	FMatchCaras() {
		bCaraSuperiorMatch = true;
		bCaraFrontalMatch = true;
		bAllEqual = true;
	}
};

USTRUCT(BlueprintType)
struct FMatchProperty {
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMatchOrientacion Orientacion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMatchCaras Caras;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MatchMinLength;

	FMatchProperty() {
		MatchMinLength = 3;
	}
};

USTRUCT(BlueprintType)
struct FMatchEventOptions {
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowMultiRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowMatchChains;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowTilesUpMatchCheck;

	FMatchEventOptions() {
		bAllowMultiRotation = false;
		bAllowMatchChains = false;
		bAllowTilesUpMatchCheck = false;
	}
};

USTRUCT(BlueprintType)
struct FMatchFound {
	GENERATED_USTRUCT_BODY();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Match Property")
	TArray<ATile*> MatchArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Match Property")
	int32 MatchSize;

	FMatchFound() {
		MatchSize = 0;
	}
};

UCLASS()
class QHEXA_UNREAL4_API AGridBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridBoard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Called when the game stops or actor is destroyed
    virtual void BeginDestroy() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;




public:
	// TILES THAT CAN BE SPAWNED
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
    TArray<FTileType> TileLibrary;

    // SIZE OF THE GRID, PLEASE, MUST BE NON > 1
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    FVector2D GridSize;

    // INITIAL ANGLE OF THE CUBES
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    int32 InitialAngle;

    // DISTANCE OF THE CAMERA
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    FVector CameraLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
    float Distance;

    UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Grid")
    TArray<ATile*> GridTilesRef;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Grid")
	ATile* CurrentTilePressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	FMatchProperty MatchCheckRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	FMatchEventOptions MatchEventOptions;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Grid")
	FVector2D InitialPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Grid")
	float EditorTimeOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid")
	TArray<ATile*> TilesActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match Arrays")
	TArray<ATile*> TilesWaitToCheckMatch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Match Arrays")
	TArray<FMatchFound> MatchesFound;


private:
	const float OFFSET = 7.05f;
	int32 LastID;




public:
    // POPULATE THE GRID
    UFUNCTION(BlueprintCallable, Category="Grid Functions")
    void PopulateTheGrid();

    // CLICK EVENT
    UFUNCTION()
    void ChangeTileSelected(ATile *TouchedActor);

    // CLEAN THE GRID
    void CleanGridBoard();

    // SPAWN A TILE WITH A GRID POSITION AND WITH A FRONT/UP FACE
    ATile* CreateTile(TSubclassOf<class ATile> tile, int8 Row, int8 Col);
    int32 SelectTileFromLibrary();

    // GET THE COMMANDS TO ROTATE THE CUBE TO GET THAT POSITION
    TArray<uint8> GetCommandsRotate(uint8 UpFace, uint8 FrontFace);

	UFUNCTION(BlueprintCallable, Category="Grid Functions")
	void AddActiveTile(ATile* activeTile);

	UFUNCTION()
	void RemoveActiveTile(ATile* activeTile);

	UFUNCTION(BlueprintCallable, Category="Grid Functions")
	bool CheckMultiRotation();

	UFUNCTION()
	int32 GetLastID();


protected:
	// METHOD THAT RETURN THE INFORMATION FOR POPULATE THE BOARD
	void GenerateBoard(uint8 dadosMatriz[7][7][2]);

	UFUNCTION()
	void CheckMatchEvent();

	void VerticalCheckMatchEvent(uint8 ColToCheck, ETileFace FaceToShow);
	void HorizontalCheckMatchEvent(uint8 RowToCheck, ETileFace FaceToShow);
	void DiagonalMatchEvent(int8 indexRow, int8 indexCol, ETileFace FaceToShow, bool bRight);
	void ProcessMatch(TArray<ATile*> &MatchInProcess, ETileFace FaceToCompare, uint8 indexCompare);
	void StoreMatch(TArray<ATile*> &Match);

	// Destroy all the matches detected.
	void CheckDestroyTile();
};

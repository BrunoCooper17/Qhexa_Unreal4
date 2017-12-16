// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FTileAbilities {
    GENERATED_USTRUCT_BODY();

    bool CanRotate() { return !bPreventRotate; }
	void SetCanRotate(bool value) { bPreventRotate = !value; }

protected:
    /* THIS PROPERTY IS USED TO MAKE THE CUBE BE ABLE OR NOT TO ROTATE */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint32 bPreventRotate : 1;

public:

};

UENUM(BlueprintType)
enum class ETileStatus : uint8 {
	TS_IDLE				UMETA(DisplayName="IDLE"),
	TS_ROTATING			UMETA(DisplayName="ROTATING"),
	TS_MATCH			UMETA(DisplayName="MATCH"),
	TS_PRE_FALL			UMETA(DisplayName="PRE_FALL"),
	TS_FALLING			UMETA(DisplayName="FALLING")
};

UENUM(BlueprintType)
enum class ETileFace : uint8 {
	TF_UP		UMETA(DisplayName="SUPERIOR"),
	TF_FRONT	UMETA(DisplayName="FRONTAL"),
	TF_BOTH		UMETA(DisplayName="BOTH")	// THIS ONE IS A SPECIAL VALUE (DON'T USE IF YOU DON'T KNOW WHAT DOES IT MEAN)
};

UCLASS()
class QHEXA_UNREAL4_API ATile : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;




public:
	// ROOT POSITION
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent *MyRoot;

	// DICE MESH
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tile")
	class UStaticMeshComponent *TileMesh;

	// ABILITIES THAT THE DICE HAVE
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category="Tile")
	FTileAbilities Properties;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tile")
	int32 Id;


protected:
	int8 *GetRotationOrientation(uint8 upFace, uint8 frontFace);

	UPROPERTY(BlueprintReadOnly, Category="Tile")
	class AGridBoard *Grid;

	FVector2D InitialFingerPosition;
	APlayerController *PC;
	float InitialTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tile")
	TArray<int32> commandsRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tile Enum")
	ETileStatus TileStatusEnum;

private:
	// VIRTUAL FACES POSITIONS
	uint8 CarasVertical[3] = {0};
	uint8 CarasHorizontal[4] = {0};

	// ALL THE POSIBILITIES OF ROTATIONS
	// UP FACE, FRONT FACE, [HORIZONTAL, VERTICAL] AXIS X=1, Y=2, Z=3
	int8 AxisSign[6][6][2] = {{{0,  0},  {-1, -2}, {-1, -3}, {-1, 3},  {-1, 2},  {0,  0}},
							  {{3,  2},  {0,  0},  {3,  -1}, {3,  1},  {0,  0},  {3,  -2}},
							  {{-2, 3},  {-2, 1},  {0,  0},  {0,  0},  {-2, -1}, {-2, -3}},
							  {{2,  -3}, {2,  -1}, {0,  0},  {0,  0},  {2,  1},  {2,  3}},
							  {{-3, -2}, {0,  0},  {-3, 1},  {-3, -1}, {0,  0},  {-3, 2}},
							  {{0,  0},  {1,  2},  {1,  3},  {1,  -3}, {1,  -2}, {0,  0}}};




public:
    // CLICK EVENT
    UFUNCTION()
    void TileSelected(AActor *TouchedActor, FKey ButtonPressed);
    UFUNCTION()
    void TileSelectedEnd(AActor* TouchedActor, FKey ButtonReleased);

    // TOUCH EVENT
    UFUNCTION()
    void TileSelectedByTouch(ETouchIndex::Type FingerIndex, AActor *ActorTouched);
    UFUNCTION()
    void TileSelectionByTouchEnd(ETouchIndex::Type FingerIndex, AActor *ActorTouched);

    // ROTATE COMMANDS (BLUEPRINT EVENT)
    // ROTATE THE CUBE, FACTOR MEANS 1=X, 2=Y, 3=Z
    UFUNCTION(BlueprintImplementableEvent, Category="Tile Events")
    void RotateCubeBP(int32 factor);
    UFUNCTION(BlueprintCallable, Category="Tile Events")
    void RotateHorizontal(bool bRight, bool instant = false);
    UFUNCTION(BlueprintCallable, Category="Tile Events")
    void RotateVertical(bool bUp, bool instant = false);

    // GET THE UPPER FACE OF THE DICE
    UFUNCTION(BlueprintCallable, Category="Tile Functions")
    int32 GetUpperFace();

    // GET THE FRONT FACE OF THE DICE
    UFUNCTION(BlueprintCallable, Category="Tile Functions")
    int32 GetFrontFace();

	UFUNCTION(BlueprintCallable, Category="Tile Functions")
	int32 GetFace(ETileFace face);

	// EVENT_ TILE ENDED ROTATION
	UFUNCTION(BlueprintCallable, Category="Tile Events")
	void RotationEnded();

	// GET THE COMMANDS TO ROTATE THE CUBE TO GET THAT POSITION
	//UFUNCTION(BlueprintCallable, Category="Tile Functions")
	TArray<int32> GetCommandsRotate(uint8 UpFace, uint8 FrontFace);

	UFUNCTION(BlueprintCallable, Category="Tile Functions")
	void SetCommandsRotate(TArray<int32> commands);

	// ROTATE THE CUBE WITH A SERIE OF COMMANDS
	UFUNCTION(BlueprintImplementableEvent, Category="Tile Events")
	void RotateWithCommands_BP(float Offset_Time);

	// ANIMATION OF DESTRUCTION
	UFUNCTION(BlueprintImplementableEvent, Category="Tile Events")
	void DestroyTile_BP();

	UFUNCTION(BlueprintCallable, Category="Tile Function")
	void SetTileStatus(ETileStatus newStatus);
	UFUNCTION(BlueprintCallable, Category="Tile Function")
	ETileStatus GetTileStatus();

private:
	void CloneFacesInfo(uint8 CarasVert[], uint8 CarasHor[]);
	void RotateVirtualVertical(bool bUp, uint8 CarasVert[], uint8 CarasHor[]);
	void RotateVirtualHorizontal(bool bRight, uint8 CarasVert[], uint8 CarasHor[]);
	void InstantRotationTile(int8 factor);
};

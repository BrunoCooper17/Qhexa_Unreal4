// Fill out your copyright notice in the Description page of Project Settings.

#include "Qhexa_Unreal4.h"
#include "GridBoard.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
    MyRoot->SetWorldLocation(FVector(0,0,0));
    RootComponent = MyRoot;

    TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    TileMesh->bVisible = true;
    TileMesh->SetRelativeLocation(FVector(0,0,0));

    if(RootComponent != nullptr) {
        TileMesh->SetupAttachment(RootComponent);
    }

    OnClicked.AddUniqueDynamic(this, &ATile::TileSelected);
    OnReleased.AddUniqueDynamic(this, &ATile::TileSelectedEnd);
    OnInputTouchBegin.AddUniqueDynamic(this, &ATile::TileSelectedByTouch);
    OnInputTouchEnd.AddUniqueDynamic(this, &ATile::TileSelectionByTouchEnd);

    // INITIALIZATION OF THE VIRTUAL FACES POSITIONS
    CarasVertical[0] = 2;
    CarasVertical[1] = 1;
    CarasVertical[2] = 5;

    CarasHorizontal[0] = 1;
    CarasHorizontal[1] = 4;
    CarasHorizontal[2] = 6;
    CarasHorizontal[3] = 3;

	TileStatusEnum = ETileStatus ::TS_IDLE;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

    Grid = Cast<AGridBoard>(GetOwner());
    PC = GEngine->GetFirstLocalPlayerController(GetWorld());
	Id = Grid->GetLastID();
}

// Called every frame
void ATile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATile::CloneFacesInfo(uint8 *CarasVert, uint8 *CarasHor) {
	for(int indexVert=0; indexVert < 3; indexVert++) {
		CarasVert[indexVert] = CarasVertical[indexVert];
	}

	for(int indexHor=0; indexHor < 4; indexHor++) {
		CarasHor[indexHor] = CarasHorizontal[indexHor];
	}
}

void ATile::RotateVirtualVertical(bool bUp, uint8 CarasVert[], uint8 CarasHor[]) {
	uint8 tmp;
	if(bUp) {
		tmp = CarasVert[0];
		for(uint8 index = 0; index < 2; index++) {
			CarasVert[index] = CarasVert[index+1];
		}
		CarasVert[2]  = CarasHor[2];
		CarasHor[2] = tmp;
	}
	else {
		tmp = CarasVert[2];
		for(int8 index = 1; index >= 0; index--) {
			CarasVert[index+1] = CarasVert[index];
		}
		CarasVert[0]  = CarasHor[2];
		CarasHor[2] = tmp;
	}
	CarasHor[0] = CarasVert[1];
}

void ATile::RotateVirtualHorizontal(bool bRight, uint8 CarasVert[], uint8 CarasHor[]) {
	uint8 tmp;
	if(bRight) {
		tmp = CarasHor[0];
		for(uint8 index = 0; index < 3; index++) {
			CarasHor[index] = CarasHor[index + 1];
		}
		CarasHor[3] = tmp;
	}
	else {
		tmp = CarasHor[3];
		for(int8 index = 2; index >= 0; index--) {
			CarasHor[index+1] = CarasHor[index];
		}
		CarasHor[0] = tmp;
	}
	CarasVert[1] = CarasHor[0];
}

void ATile::InstantRotationTile(int8 factor) {
	int tmpAngle = (90 * factor)/(FMath::Abs(factor));

	switch (FMath::Abs(factor)) {
		case 1:
			AddActorLocalRotation(FRotator(0.0f, 0.0f, tmpAngle));
			break;
		case 2:
			AddActorLocalRotation(FRotator(tmpAngle, 0.0f, 0.0f));
			break;
		case 3:
			AddActorLocalRotation(FRotator(0.0f, tmpAngle, 0.0f));
			break;
	}
}

void ATile::TileSelected(AActor *TouchedActor, FKey ButtonPressed) {
	Grid->ChangeTileSelected(this);
}

void ATile::TileSelectedEnd(AActor *TouchedActor, FKey ButtonReleased) {

}

void ATile::TileSelectedByTouch(ETouchIndex::Type FingerIndex, AActor *ActorTouched) {
	Grid->ChangeTileSelected(this);
}

void ATile::TileSelectionByTouchEnd(ETouchIndex::Type FingerIndex, AActor *ActorTouched) {

}

inline int8* ATile::GetRotationOrientation(uint8 upFace, uint8 frontFace) {
    return AxisSign[upFace][frontFace];
}

void ATile::RotateHorizontal(bool bRight, bool instant/* = false*/) {
    int8 factor = 0;

	if(TileStatusEnum != ETileStatus::TS_IDLE) return;
	Grid->AddActiveTile(this);

    int8* axisSigns = GetRotationOrientation(GetUpperFace() -1 , GetFrontFace() - 1);
	RotateVirtualHorizontal(bRight, CarasVertical, CarasHorizontal);

    if(bRight) {
        factor = axisSigns[0];
    }
    else {
        factor = -axisSigns[0];
    }
    CarasVertical[1] = CarasHorizontal[0];

	if(!instant) {
		SetTileStatus(ETileStatus::TS_ROTATING);
		RotateCubeBP(factor);
	}
	else {
		InstantRotationTile(factor);
	}
}

void ATile::RotateVertical(bool bUp, bool instant/* = false*/) {
    int8 factor = 0;

	if(TileStatusEnum != ETileStatus::TS_IDLE) return;
	Grid->AddActiveTile(this);

    int8* axisSigns = GetRotationOrientation(GetUpperFace() -1 , GetFrontFace() - 1);
	RotateVirtualVertical(bUp, CarasVertical, CarasHorizontal);

    if(bUp) {
        factor = axisSigns[1];
    }
    else {
        factor = -axisSigns[1];
    }
    CarasHorizontal[0] = CarasVertical[1];

	if(!instant) {
		SetTileStatus(ETileStatus::TS_ROTATING);
		RotateCubeBP(factor);
	}
	else {
		InstantRotationTile(factor);
	}
}

int32 ATile::GetUpperFace() {
    return CarasVertical[0];
}

int32 ATile::GetFrontFace() {
    return CarasHorizontal[0];
}

int32 ATile::GetFace(ETileFace face) {
	return face == ETileFace ::TF_UP ? CarasVertical[0] : CarasHorizontal[0];
}

void ATile::RotationEnded() {
	//UE_LOG(LogClass, Log, TEXT("ROTATION ENDED %s"), *GetName());
	SetTileStatus(ETileStatus::TS_IDLE);
	Grid->RemoveActiveTile(this);
}

// GETT THE COMMAND TO ROTATE THE TILE (1 MEANS VERTICAL, 2 MEANS HORIZONTAL)
TArray<int32> ATile::GetCommandsRotate(uint8 UpFace, uint8 FrontFace) {
	uint8 CarasVert[3] = {0};
	uint8 CarasHor[4] = {0};
	TArray<int32> commands;

	CloneFacesInfo(CarasVert, CarasHor);

	// SEARCH THE UP NUMBER (IN THE VERTICAL POSITION)
	bool bUpFace = false;
	int8 indexVert = 0;

	for(; indexVert < 3 && CarasVert[indexVert] != UpFace; indexVert++ ) ;
	if(indexVert < 3 || CarasHor[2] == UpFace) { bUpFace = true; }

	if(bUpFace) {
VERTICAL:
		// THE UP NUMBER WHERE FOUND IN THE VERTICAL POSITION
		while(CarasVert[0] != UpFace) {
			RotateVirtualVertical(true, CarasVert, CarasHor);
			commands.Add(1);
		}
	}
	else {
		// THE UP NUMBER IS IN THE HORIZONTAL POSITION
		while(CarasHor[0] != UpFace) {
			RotateVirtualHorizontal(true, CarasVert, CarasHor);
			commands.Add(2);
		}
		goto VERTICAL;
	}

	// THE FRONT FACE IT'S NOW INT HE HORIZONTAL POSITION
	while(CarasHor[0] != FrontFace) {
		RotateVirtualHorizontal(true, CarasVert, CarasHor);
		commands.Add(2);
	}

    return commands;
}

void ATile::SetCommandsRotate(TArray<int32> commands) {
	commandsRotate = commands;
}

void ATile::SetTileStatus(ETileStatus newStatus) {
	TileStatusEnum = newStatus;
}

ETileStatus ATile::GetTileStatus() {
	return TileStatusEnum;
}
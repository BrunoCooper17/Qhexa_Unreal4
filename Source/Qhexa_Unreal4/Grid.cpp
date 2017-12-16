// Fill out your copyright notice in the Description page of Project Settings.

#include "Qhexa_Unreal4.h"
#include "Grid.h"


// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MinimumRunLenght = 3;
	CubeTileSize.Set(25.0f, 25.0f);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
    float Angle = 0;
    int32 RowIndex, ColIndex;
    RowIndex = FMath::FloorToInt(GridHeight/2.0f);
    ColIndex = FMath::FloorToInt(GridWidth/2.0f);

	Super::BeginPlay();

    if(GEngine) {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("GRID C++") );
        CameraLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
        Distance = FMath::Sqrt(FMath::Pow(CameraDistance.X, 2.0f) + FMath::Pow(CameraDistance.Y, 2.0f));
    }

    UWorld* const World = GetWorld();
    FVector Position(0.0f, 0.0f, 0.0f);
    FRotator Rotation(0.0f, 0.0f, 0.0f);
    FActorSpawnParameters SpawnParams;

    SpawnParams.Owner = this;
    SpawnParams.Instigator = Instigator;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if(World) {
        for (int rows = -RowIndex; rows <= RowIndex; rows++) {
            float AngleDeg = InitAngle + OFFSET * rows;
            Angle = FMath::DegreesToRadians(AngleDeg);
            for (int cols = -ColIndex; cols <= ColIndex; cols++) {

                Position.X = Distance * FMath::Cos(Angle) + CameraLocation.X + CameraDistance.X;
                Position.Y = 100 * cols + CameraLocation.Y;
                Position.Z = Distance * FMath::Sin(Angle) + CameraLocation.Z + CameraDistance.Y;

                Rotation.Roll = 0.0f;                   // X
                Rotation.Pitch = AngleDeg - InitAngle;  // Y
                Rotation.Yaw = 0.0f;                    // Z

                ACubeTile* tmpCube = World->SpawnActor<ACubeTile>(CubeTileToSpaw, Position, Rotation, SpawnParams);
                tmpCube->RotateHorizontal(true);
                tmpCube->RotateVertical(true);
            }
        }
    }
}

// Called every frame
void AGrid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

ACubeTile * AGrid::CreateCubeTile(AActor * TileCube, FVector SpawnLocation, int32 SpawnLocationOnWorld) {
	return nullptr;
}

int32 AGrid::SelectCubeTileFromLibrary() {
	return 0;
}

ACubeTile * AGrid::GetCubeTileFromGridAddress(int32 GridAddress) {
	return nullptr;
}

/*
void AGrid::OnMoveMade() {

}
*/

FVector AGrid::GetLocationFromGridAddres(int32 GridAddress) {
	FVector tmpVector;
	return tmpVector;
}

FVector AGrid::GetLocationFromGridAddresWhitOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) {
	FVector tmpVector;
	return tmpVector;
}

bool AGrid::GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset) {
	return false;
}

bool AGrid::AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) {
	return false;
}

void AGrid::OnCubeTileFinishedFalling(ACubeTile* Tile, int32 LandingGridAddress) {

}

void AGrid::OnCubeTileFinishedMatching(ACubeTile* InTile) {

}
void AGrid::OnCubeTileFinishedRotating(ACubeTile* InTile) {

}

void AGrid::RespawnCubeTiles() {

}

void AGrid::RotateCube(ACubeTile* CubeTile) {

}

bool AGrid::IsMoveLegal(ACubeTile* CubeTile) {
	return false;
}

TArray<ACubeTile*> AGrid::FindNeighbors(ACubeTile* StartingCubeTile, bool bMustMatchID, int32 RunLenght) {
	TArray<ACubeTile*> tmpArray;
	return tmpArray;
}

TArray<ACubeTile*> AGrid::FindCubeTilesOfType(int32 TileTypeID) {
	TArray<ACubeTile*> tmpArray;
	return tmpArray;
}

void AGrid::ExecuteMatch(TArray<ACubeTile*> MatchingTiles) {

}

void AGrid::OnTileWasSelected(ACubeTile* NewSelectedTile) {

}


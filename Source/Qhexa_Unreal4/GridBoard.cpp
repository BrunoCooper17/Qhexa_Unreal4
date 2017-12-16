// Fill out your copyright notice in the Description page of Project Settings.

#include "Qhexa_Unreal4.h"
#include "GridBoard.h"

// Sets default values
AGridBoard::AGridBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    GridSize.X = 7;
    GridSize.Y = 7;

    InitialAngle = 225;
	EditorTimeOffset = 0.035f;
	LastID = 0;

	CurrentTilePressed = nullptr;
}

// Called when the game starts or when spawned
void AGridBoard::BeginPlay()
{
	Super::BeginPlay();
}

void AGridBoard::BeginDestroy() {
    Super::BeginDestroy();

    CleanGridBoard();
}

// Called every frame
void AGridBoard::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGridBoard::PopulateTheGrid() {
    float Angle = 0;
    int32 RowIndex, ColIndex;
    RowIndex = FMath::FloorToInt(GridSize.Y/2.0f);
    ColIndex = FMath::FloorToInt(GridSize.X/2.0f);

    CameraLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
    Distance = FMath::Sqrt(FMath::Pow(CameraLocation.X, 2.0f) + FMath::Pow(CameraLocation.Z, 2.0f));

	float timeOffset = 0.0f;

	uint8 InfoGrid[7][7][2] = {{{0,0}}};
	GenerateBoard(InfoGrid);

    for (int rows = -RowIndex; rows <= RowIndex; rows++) {
        for (int cols = -ColIndex; cols <= ColIndex; cols++) {
            int32 tileIndex = SelectTileFromLibrary();
            ATile* tmpCube = CreateTile(TileLibrary[tileIndex].TileToSpaw, rows, cols);
            if(tmpCube) {
				TArray<int32> commands = tmpCube->GetCommandsRotate(InfoGrid[rows + RowIndex][cols + ColIndex][0], InfoGrid[rows + RowIndex][cols + ColIndex][1]);

				tmpCube->SetCommandsRotate(commands);
				tmpCube->RotateWithCommands_BP(timeOffset);
                GridTilesRef.Add(tmpCube);

				timeOffset += EditorTimeOffset;
            }
        }
    }
}

void AGridBoard::ChangeTileSelected(ATile *ActorTouched) {
	if(ActorTouched && ActorTouched->GetTileStatus() == ETileStatus::TS_IDLE && !CurrentTilePressed)
		CurrentTilePressed = ActorTouched;
}

void AGridBoard::CleanGridBoard() {
    UE_LOG(LogClass, Log, TEXT("DESTROY GRID CALLED"));
    for(auto& tile : GridTilesRef) {
        tile->Destroy();
    }
}

ATile* AGridBoard::CreateTile(TSubclassOf<class ATile> tile, int8 Row, int8 Col) {
    UWorld* const World = GetWorld();
    FVector Position(0.0f, 0.0f, 0.0f);
    FRotator Rotation(0.0f, 0.0f, 0.0f);
    FActorSpawnParameters SpawnParams;

    SpawnParams.Owner = this;
    SpawnParams.Instigator = Instigator;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if(World) {
        float AngleDeg = InitialAngle + OFFSET * Row;
        float Angle = FMath::DegreesToRadians(AngleDeg);

        Position.X = Distance * FMath::Cos(Angle) + CameraLocation.X;// + CameraDistance.X;
        Position.Y = -100 * Col + CameraLocation.Y;
        Position.Z = Distance * FMath::Sin(Angle) + CameraLocation.Z;// + CameraDistance.Y;

        Rotation.Roll = 0.0f;                      // X
        Rotation.Pitch = AngleDeg - InitialAngle;  // Y
        Rotation.Yaw = 0.0f;                       // Z

        return World->SpawnActor<ATile>(tile, Position, Rotation, SpawnParams);
    }

    return nullptr;
}

int32 AGridBoard::SelectTileFromLibrary() {
    return 0;
}

TArray<uint8> AGridBoard::GetCommandsRotate(uint8 UpFace, uint8 FrontFace) {
    TArray<uint8> commands;
    return commands;
}

// THE UPFACE INDEX IS 0, THE FRONTFACE INDEX IS 1
void AGridBoard::GenerateBoard(uint8 dadosMatriz[7][7][2]) {
	for(uint8 rowIndex=0; rowIndex < GridSize.Y; rowIndex++) {
		for(uint8 colIndex=0; colIndex < GridSize.X; colIndex++) {
			// Random Numbers
			uint8 upFace = 0, frontFace = 0;

			// What kind of match are we going to find
			// By default, are false in case the match is not found
			bool bMatchFound;

			do {
				bMatchFound = false;

				// Select one position randomly
				do{
					upFace = FMath::RandRange(1,6);
					frontFace = FMath::RandRange(1,6);
				}while(upFace == frontFace || (upFace + frontFace) == 7);

				// CHECK HORIZONTAL MATCH
				if(MatchCheckRules.Orientacion.bHorizontalMatch && colIndex >= MatchCheckRules.MatchMinLength-1) {
					uint8 coincidenciasArriba = 1;
					uint8 coincidenciasAbajo = 1;
					uint8 continueFlag;
					for(int index=1; index < MatchCheckRules.MatchMinLength; index++) {
						continueFlag = 0;
						if(MatchCheckRules.Caras.bCaraSuperiorMatch && dadosMatriz[rowIndex][colIndex - index][0] == upFace){
							coincidenciasArriba++;
							continueFlag++;
						}
						if(MatchCheckRules.Caras.bCaraFrontalMatch && dadosMatriz[rowIndex][colIndex - index][1] == frontFace){
							coincidenciasAbajo++;
							continueFlag++;
						}

						if(!continueFlag) break;
					}

					if(coincidenciasArriba == MatchCheckRules.MatchMinLength || coincidenciasAbajo == MatchCheckRules.MatchMinLength) {
						bMatchFound = true;
						//UE_LOG(LogClass, Log, TEXT("FOUND MATCH HORIZONTAL %d %d"), coincidenciasArriba == MatchCheckRules.MatchMinLength, coincidenciasAbajo == MatchCheckRules.MatchMinLength);
						continue;
					}
				}

				// CHECK FOR VERTICAL MATCH
				if(MatchCheckRules.Orientacion.bVerticalMatch && rowIndex >= MatchCheckRules.MatchMinLength-1) {
					uint8 coincidenciasArriba = 1;
					uint8 coincidenciasAbajo = 1;
					uint8 continueFlag;
					for(int index=1; index < MatchCheckRules.MatchMinLength; index++) {
						continueFlag = 0;
						if(MatchCheckRules.Caras.bCaraSuperiorMatch && dadosMatriz[rowIndex - index][colIndex][0] == upFace){
							coincidenciasArriba++;
							continueFlag++;
						}
						if(MatchCheckRules.Caras.bCaraFrontalMatch && dadosMatriz[rowIndex - index][colIndex][1] == frontFace){
							coincidenciasAbajo++;
							continueFlag++;
						}

						if(!continueFlag) break;
					}

					if(coincidenciasArriba == MatchCheckRules.MatchMinLength || coincidenciasAbajo == MatchCheckRules.MatchMinLength) {
						bMatchFound = true;
						//UE_LOG(LogClass, Log, TEXT("FOUND MATCH VERTICAL %d %d"), coincidenciasArriba == MatchCheckRules.MatchMinLength, coincidenciasAbajo == MatchCheckRules.MatchMinLength);
						continue;
					}
				}

				// CHECK FOR DIAGONAL MATCH
				if(MatchCheckRules.Orientacion.bDiagonalMatch && rowIndex >= MatchCheckRules.MatchMinLength-1) {
					if(colIndex >= MatchCheckRules.MatchMinLength-1) {
						uint8 coincidenciasArriba = 1;
						uint8 coincidenciasAbajo = 1;
						uint8 continueFlag;
						for(int index=1; index < MatchCheckRules.MatchMinLength; index++) {
							continueFlag = 0;
							if(MatchCheckRules.Caras.bCaraSuperiorMatch && dadosMatriz[rowIndex - index][colIndex - index][0] == upFace){
								coincidenciasArriba++;
								continueFlag++;
							}
							if(MatchCheckRules.Caras.bCaraFrontalMatch && dadosMatriz[rowIndex - index][colIndex - index][1] == frontFace){
								coincidenciasAbajo++;
								continueFlag++;
							}

							if(!continueFlag) break;
						}

						if(coincidenciasArriba == MatchCheckRules.MatchMinLength || coincidenciasAbajo == MatchCheckRules.MatchMinLength) {
							bMatchFound = true;
							//UE_LOG(LogClass, Log, TEXT("FOUND MATCH DIAGONAL_! %d %d"), coincidenciasArriba == MatchCheckRules.MatchMinLength, coincidenciasAbajo == MatchCheckRules.MatchMinLength);
							continue;
						}
					}

					if(colIndex < (GridSize.X - (MatchCheckRules.MatchMinLength - 1))) {
						uint8 coincidenciasArriba = 1;
						uint8 coincidenciasAbajo = 1;
						uint8 continueFlag;
						for(int index=1; index < MatchCheckRules.MatchMinLength; index++) {
							continueFlag = 0;
							if(MatchCheckRules.Caras.bCaraSuperiorMatch && dadosMatriz[rowIndex - index][colIndex + index][0] == upFace){
								coincidenciasArriba++;
								continueFlag++;
							}
							if(MatchCheckRules.Caras.bCaraFrontalMatch && dadosMatriz[rowIndex - index][colIndex + index][1] == frontFace){
								coincidenciasAbajo++;
								continueFlag++;
							}

							if(!continueFlag) break;
						}

						if(coincidenciasArriba == MatchCheckRules.MatchMinLength || coincidenciasAbajo == MatchCheckRules.MatchMinLength) {
							bMatchFound = true;
							//UE_LOG(LogClass, Log, TEXT("FOUND MATCH DIAGONAL_2 %d %d"), coincidenciasArriba == MatchCheckRules.MatchMinLength, coincidenciasAbajo == MatchCheckRules.MatchMinLength);
							continue;
						}
					}
				}

				// Repeat the process in case one kind of match is found
			}while(bMatchFound);

			dadosMatriz[rowIndex][colIndex][0] = upFace;
			dadosMatriz[rowIndex][colIndex][1] = frontFace;
		}
	}
}

void AGridBoard::AddActiveTile(ATile *activeTile) {
	TilesActive.Add(activeTile);
}

void AGridBoard::RemoveActiveTile(ATile *activeTile) {
	TilesActive.Remove(activeTile);
	TilesWaitToCheckMatch.Add(activeTile);

	if(!TilesActive.Num()) {
		MatchesFound.Empty();
		CheckMatchEvent();
		CheckDestroyTile();
	}
}

bool AGridBoard::CheckMultiRotation() {
	if(!MatchEventOptions.bAllowMultiRotation) {
		if(TilesActive.Num())
			return false;
		return true;
	}
	return true;
}

int32 AGridBoard::GetLastID() {
	return (LastID++);
}

void AGridBoard::CheckMatchEvent() {
	UE_LOG(LogClass, Log, TEXT("CHECKING FOR MATCH..."));

	TArray<uint8> ColsCheckingList;
	TArray<uint8> RowsCheckingList;
	TArray<uint8> DiagonalRightCheckingList;
	TArray<uint8> DiagonalLeftCheckingList;

	const int maxDiagonalCheck = GridSize.X + GridSize.Y - 2 -(MatchCheckRules.MatchMinLength - 1);

	int32 tilesToCheck = TilesWaitToCheckMatch.Num();
	for(int32 index=0; index<tilesToCheck; index++) {
		int32 TileIndexInGrid = GridTilesRef.Find(TilesWaitToCheckMatch[index]);
		uint8 tileRow = (uint8)(TileIndexInGrid / GridSize.Y);
		uint8 tileCol = TileIndexInGrid % (int32)GridSize.X;

		if(MatchCheckRules.Orientacion.bHorizontalMatch)
			ColsCheckingList.AddUnique(tileCol);
		if(MatchCheckRules.Orientacion.bVerticalMatch)
			RowsCheckingList.AddUnique(tileRow);

		if(!MatchCheckRules.Orientacion.bDiagonalMatch) continue;

		uint8 tmpDiagonalCheck = tileCol + tileRow;
		if( tmpDiagonalCheck >= MatchCheckRules.MatchMinLength-1 && tmpDiagonalCheck <= maxDiagonalCheck)
			DiagonalRightCheckingList.AddUnique(tmpDiagonalCheck);
		tmpDiagonalCheck = tileCol + (GridSize.Y - tileRow - 1);
		if(tmpDiagonalCheck >= MatchCheckRules.MatchMinLength-1 && tmpDiagonalCheck <= maxDiagonalCheck)
			DiagonalLeftCheckingList.AddUnique(tmpDiagonalCheck);
	}

	int32 LinesToCheck = ColsCheckingList.Num();
	for(int32 index=0; index<LinesToCheck; index++) {
		uint8 ColToCheck = ColsCheckingList[index];
		//UE_LOG(LogClass, Log, TEXT("CHECKING VERTICAL %d"), ColToCheck);

		if(MatchCheckRules.Caras.bAllEqual) {
			VerticalCheckMatchEvent(ColToCheck, ETileFace::TF_BOTH);
		} else {
			if(MatchCheckRules.Caras.bCaraSuperiorMatch)
				VerticalCheckMatchEvent(ColToCheck, ETileFace::TF_UP);
			if(MatchCheckRules.Caras.bCaraFrontalMatch)
				VerticalCheckMatchEvent(ColToCheck, ETileFace::TF_FRONT);
		}
	}

	LinesToCheck = RowsCheckingList.Num();
	for(int32 index=0; index<LinesToCheck; index++) {
		uint8 RowToCheck = RowsCheckingList[index];
		//UE_LOG(LogClass, Log, TEXT("CHECKING HORIZONTAL %d"), RowToCheck);

		if(MatchCheckRules.Caras.bAllEqual) {
			HorizontalCheckMatchEvent(RowToCheck, ETileFace::TF_BOTH);
		} else {
			if(MatchCheckRules.Caras.bCaraSuperiorMatch)
				HorizontalCheckMatchEvent(RowToCheck, ETileFace::TF_UP);
			if(MatchCheckRules.Caras.bCaraFrontalMatch)
				HorizontalCheckMatchEvent(RowToCheck, ETileFace::TF_FRONT);
		}
	}

	LinesToCheck = DiagonalRightCheckingList.Num();
	for(int32 index=0; index<LinesToCheck; index++) {
		int8 indexRow, indexCol;
		//UE_LOG(LogClass, Log, TEXT("DIAGONAL DERECHA %d"), DiagonalLeftCheckingList[index]);

		indexRow = DiagonalRightCheckingList[index] >= GridSize.Y ? GridSize.Y - 1 : DiagonalRightCheckingList[index];
		indexCol = DiagonalRightCheckingList[index] >= GridSize.Y ? DiagonalRightCheckingList[index] - (GridSize.Y - 1) : 0;

		UE_LOG(LogClass, Log, TEXT("DIAGONAL DERECHA %d %d %d"), DiagonalLeftCheckingList[index], indexRow, indexCol);

		if(MatchCheckRules.Caras.bAllEqual) {
			DiagonalMatchEvent(indexRow, indexCol, ETileFace::TF_BOTH, true);
		} else {
			if(MatchCheckRules.Caras.bCaraSuperiorMatch)
				DiagonalMatchEvent(indexRow, indexCol, ETileFace::TF_UP, true);
			if(MatchCheckRules.Caras.bCaraFrontalMatch)
				DiagonalMatchEvent(indexRow, indexCol, ETileFace::TF_FRONT, true);
		}
	}

	LinesToCheck = DiagonalLeftCheckingList.Num();
	for(int32 index=0; index<LinesToCheck; index++) {
		int8 indexRow, indexCol;
		//UE_LOG(LogClass, Log, TEXT("DIAGONAL IZQUIERDA %d"), DiagonalLeftCheckingList[index]);

		indexCol = DiagonalLeftCheckingList[index] >= GridSize.Y ? GridSize.Y - 1 : DiagonalLeftCheckingList[index];
		indexRow = DiagonalLeftCheckingList[index] >= GridSize.Y ? (GridSize.X - 1) - (DiagonalLeftCheckingList[index] - (GridSize.Y -1)) : GridSize.X - 1;

		if(MatchCheckRules.Caras.bAllEqual) {
			DiagonalMatchEvent(indexRow, indexCol, ETileFace::TF_BOTH, false);
		} else {
			if(MatchCheckRules.Caras.bCaraSuperiorMatch)
				DiagonalMatchEvent(indexRow, indexCol, ETileFace::TF_UP, false);
			if(MatchCheckRules.Caras.bCaraFrontalMatch)
				DiagonalMatchEvent(indexRow, indexCol, ETileFace::TF_FRONT, false);
		}
	}

	TilesWaitToCheckMatch.Empty();
}

void AGridBoard::VerticalCheckMatchEvent(uint8 ColToCheck, ETileFace FaceToShow) {
	TArray<ATile *> MatchInProcess;
	MatchInProcess.Add(GridTilesRef[ColToCheck]);

	for(uint8 indexRow=1; indexRow < GridSize.Y; indexRow++){
		ProcessMatch(MatchInProcess, FaceToShow, indexRow * GridSize.Y + ColToCheck);
	}

	// COLUMN ENDED. LAST CHECK
	StoreMatch(MatchInProcess);
}

void AGridBoard::HorizontalCheckMatchEvent(uint8 RowToCheck, ETileFace FaceToShow) {
	TArray<ATile *> MatchInProcess;
	RowToCheck *= GridSize.Y;
	MatchInProcess.Add(GridTilesRef[RowToCheck]);

	for(uint8 indexCol=1; indexCol < GridSize.X; indexCol++){
		ProcessMatch(MatchInProcess, FaceToShow, RowToCheck + indexCol);
	}

	// COLUMN ENDED. LAST CHECK
	StoreMatch(MatchInProcess);
}

void AGridBoard::DiagonalMatchEvent(int8 indexRow, int8 indexCol, ETileFace FaceToShow, bool bRight) {
	TArray<ATile *> MatchInProcess;
	MatchInProcess.Add(GridTilesRef[indexRow * GridSize.Y + indexCol]);

	indexRow--;
	indexCol = bRight ? indexCol + 1 : indexCol - 1;
	bool bColCondition = bRight ? indexCol < GridSize.X : indexCol >= 0;

	while(indexRow >= 0 && bColCondition) {
		ProcessMatch(MatchInProcess, FaceToShow, indexRow * GridSize.Y + indexCol);
		indexRow--;
		indexCol = bRight ? indexCol + 1 : indexCol - 1;
		bColCondition = bRight ? indexCol < GridSize.X : indexCol >= 0;
	}

	// COLUMN ENDED. LAST CHECK
	StoreMatch(MatchInProcess);
}

void AGridBoard::ProcessMatch(TArray<ATile*> &MatchInProcess, ETileFace FaceToCompare, uint8 indexCompare) {
	bool bCompareFaces;
	if(FaceToCompare == ETileFace::TF_BOTH) {
		bCompareFaces = MatchInProcess[0]->GetUpperFace() == GridTilesRef[indexCompare]->GetUpperFace() &&
						MatchInProcess[0]->GetFrontFace() == GridTilesRef[indexCompare]->GetFrontFace();
	} else {
		bCompareFaces = (MatchInProcess[0]->GetFace(FaceToCompare) == GridTilesRef[indexCompare]->GetFace(FaceToCompare));
	}

	if(bCompareFaces) {
		MatchInProcess.Add(GridTilesRef[indexCompare]);
	} else {
		// MATCH INTERRUMPETD
		StoreMatch(MatchInProcess);
		MatchInProcess.Empty();
		MatchInProcess.Add(GridTilesRef[indexCompare]);
	}
}

void AGridBoard::StoreMatch(TArray<ATile *> &Match) {
	if(Match.Num() >= MatchCheckRules.MatchMinLength) {
		UE_LOG(LogClass, Log, TEXT("MATCH FOUND"));
		FMatchFound tmpMatch;
		tmpMatch.MatchSize = Match.Num();
		for(uint8 tmpIndex=0; tmpIndex<tmpMatch.MatchSize; tmpIndex++){
			tmpMatch.MatchArray.Add(Match[tmpIndex]);
			//UE_LOG(LogClass, Log, TEXT("TILE MATCH -> %d %d"), Match[tmpIndex]->GetUpperFace(), Match[tmpIndex]->GetFrontFace());
		}

		MatchesFound.Add(tmpMatch);
	}
}

void AGridBoard::CheckDestroyTile() {
	UE_LOG(LogClass, Log, TEXT("DESTROYING %d MATCHES"), MatchesFound.Num());
	for(uint8 Index=0, MatchCount = MatchesFound.Num(); Index<MatchCount; Index++) {
		for (uint8 indexTile=0; indexTile<MatchesFound[Index].MatchSize; indexTile++) {
			MatchesFound[Index].MatchArray[indexTile]->DestroyTile_BP();
		}
	}
}

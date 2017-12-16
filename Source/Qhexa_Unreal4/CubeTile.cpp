// Fill out your copyright notice in the Description page of Project Settings.

#include "Qhexa_Unreal4.h"
#include "CubeTile.h"

// Sets default values
ACubeTile::ACubeTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


}

// Called when the game starts or when spawned
void ACubeTile::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddUniqueDynamic(this, &ACubeTile::TileSelected);
	OnInputTouchBegin.AddUniqueDynamic(this, &ACubeTile::TileSelectedByTouch);

    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("CUBE C++") );
}

// Called every frame
void ACubeTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}





void ACubeTile::TileSelected(AActor* sender, FKey key) {
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("CUBE CLICKED") );
}

void ACubeTile::TileSelectedByTouch(ETouchIndex::Type FingerIndex, AActor* sender) {
	TileSelected(this, FKey());
    //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("CUBE CLICKED") );
}

void ACubeTile::StartFalling(bool bUseCurrentWorldLocation) {

}

void ACubeTile::TickFalling() {

}

void ACubeTile::FinishFalling() {

}

void ACubeTile::SetGridAddress(int32 newLocation) {

}

int32 ACubeTile::GetGridAddress() const {
	return 0;
}
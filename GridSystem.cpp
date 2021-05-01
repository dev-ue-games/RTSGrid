// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSystem.h"

// Sets default values
AGridSystem::AGridSystem()

	: GridDimensions(FGridCoord(4))
	, CellSize(100.0)
	, bShowPreviewGrid(true)
	, bShowTileTextInfo(false)
	, bDrawBoundingBox(true)


{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TArray<FGridCoord> AGridSystem::GenerateGrid()
{
	// this is the data. Makes sure its empty
	GeneratedGrid.Empty();

	// epic says this is naughty but we did it anyway
	int xMin, xMax, yMin, yMax;

	xMin = 0; // -5
	xMax = GridDimensions.Row; // 10
	yMin = 0; // 5 this generates a centered grid
	yMax = GridDimensions.Col;

	for (int i = xMin; i < xMax; i++)
	{
		for (int j = yMin; j < yMax; j++)
		{
			FGridCoord Coord;
			Coord.Row = i;
			Coord.Col = j;
			// Add makes a copy. emplace is cheaper
			GeneratedGrid.Emplace(Coord);
		};
	};

	//GenerateVisualGrid();
	return GeneratedGrid;

}
// Called when the game starts or when spawned
void AGridSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


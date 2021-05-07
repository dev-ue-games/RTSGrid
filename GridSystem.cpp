/ Fill out your copyright notice in the Description page of Project Settings.


#include "GridSystem.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	PreviewGridHISM = CreateDefaultSubobject < UHierarchicalInstancedStaticMeshComponent>(TEXT("PreviewGrid"));

	PreviewGridHISM->SetupAttachment(RootComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh>PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));

		if (PlaneMesh.Succeeded())
		{
			PreviewGridHISM->SetStaticMesh(PlaneMesh.Object);
		}


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
FVector AGridSystem::GetGridOriginRelative()
{
	FVector Result;
	const float CellHalfSize = CellSize * 0.5;

	Result.X = ((GridDimensions.Row * 0.5) * CellSize) - CellHalfSize;
	Result.Y = ((GridDimensions.Col * 0.5) * CellSize) - CellHalfSize;
	Result.Z = 0.0;
	return FVector();
}

FVector AGridSystem::GetGridOriginWorld()
{
	return GetGridOriginRelative() + GetActorLocation() ;
}

FVector2D AGridSystem::GetGridSize()
{
	return FVector2D(GridDimensions.Row * CellSize, GridDimensions.Col * CellSize);
}

FVector2D AGridSystem::GetGridExtents()
{
	return GetGridSize() * 0, 5;
}

FVector AGridSystem::GetGridRelativeFromWorld(FVector WorldLocation)
{
	return WorldLocation - GetActorLocation();
}

FVector AGridSystem::GetCellCenterFromRelative(FVector RelativeLocation, bool bReturnWorldSpace)
{
	int ID;
	const FGridCoord Coord = GetCoordinateFromRelative(RelativeLocation, ID);

	FVector CellCenter;

	CellCenter.X = Coord.Row * CellSize;
	CellCenter.Y = Coord.Col * CellSize;
	CellCenter.Z = 0.0;

	return bReturnWorldSpace ? CellCenter + GetActorLocation() : CellCenter;;
}

bool AGridSystem::IsInGridBounds(FGridCoord Coordinate)
{
	return (Coordinate >= FGridCoord(0.0)&& Coordinate < GridDimensions);
}

bool AGridSystem::IsClearTile(FGridCoord Coordinate)
{
	return !BlockedTiles.Contains(Coordinate);
}

bool AGridSystem::IsValidLocation(FGridCoord Cordinate)
{
	return IsInGridBounds(Cordinate) && IsClearTile(Cordinate);
}

FGridCoord AGridSystem::GetCoordinateFromRelative(FVector RelativeLocation, int& CellID)
{
	FGridCoord Coord;
	Coord.Col = FMath::RoundToInt(RelativeLocation.Y / CellSize);
	Coord.Row = FMath::RoundToInt(RelativeLocation.X / CellSize);
	CellID = GetCellIDFromCoordinate(Coord);
	return Coord;
}

FGridCoord AGridSystem::GetCoordinateFromCellID(int ID)
{
	FGridCoord Coord;
	Coord.Col = ID / GridDimensions.Row;
	Coord.Row = ID % GridDimensions.Row;
	return Coord;
}

int AGridSystem::GetCellIDFromCoordinate(FGridCoord Coordinate)
{
	return (GridDimensions.Row * Coordinate.Col)+Coordinate.Row;
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

void AGridSystem::GenerateVisualGrid()
{
	if (!bShowPreviewGrid == false)
	{
		PreviewGridHISM->ClearInstances();
	}
	for (auto* text : TextComponents)
	{

		text->DestroyComponent();

	}
	TextComponents.Empty();

	PreviewGridHISM->ClearInstances();

	PreviewGridHISM->bAutoRebuildTreeOnInstanceChanges = false;

	for (int i = 0; i < GeneratedGrid.Num(); i++)
	{
		FGridCoord CurrentTile = GeneratedGrid[i];
		if (bShowPreviewGrid && PreviewGridHISM)
		{
			const FVector TileLocation = FVector(CurrentTile.Row * CellSize, CurrentTile.Col * CellSize, 0.0);
			const FTransform T = FTransform(FRotator::ZeroRotator, TileLocation, FVector(CellSize * 0.01));

			PreviewGridHISM->AddInstance(T);


		}

		if (bShowTileTextInfo)
		{
			UTextRenderComponent* Text = NewObject<UTextRenderComponent>(this);
			TextComponents.Add(Text);

			if (Text)
			{
				Text->RegisterComponent();


				Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
				Text->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;


				const FVector TileLocation = FVector(CurrentTile.Row * CellSize, CurrentTile.Col * CellSize, 0.0);
				const FTransform T = FTransform(FRotator (90,0,90), TileLocation, FVector::OneVector);

				Text->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				Text->SetRelativeTransform(T);

				FFormatNamedArguments Args;
				Args.Add("x", CurrentTile.Row);
				Args.Add("y", CurrentTile.Col);
				Args.Add("id", i);
				FText ShowCoords = FText::Format(NSLOCTEXT("gridsys", "DebugTextCoords", "X:{x},Y:{y},\n ID:{id}"), Args);
				Text->SetText(ShowCoords);

			}



		}



	}
	PreviewGridHISM->bAutoRebuildTreeOnInstanceChanges = true;
	PreviewGridHISM->BuildTreeIfOutdated(true, true);

	UKismetSystemLibrary::FlushPersistentDebugLines(this);
	if (bDrawBoundingBox)
	{
		
		UKismetSystemLibrary::DrawDebugBox(this, GetGridOriginWorld(), FVector(GetGridExtents(), 50.0), FLinearColor::Red, FRotator::ZeroRotator, 3000.0, 2.0);
	}
	


}

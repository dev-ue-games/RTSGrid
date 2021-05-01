// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCoords.h"

bool UGridCoordinateLibraary::Equal_CoordCoord(FGridCoord A, FGridCoord B)
{
	return A == B;
}

FGridCoord UGridCoordinateLibraary::Multiply_CoordFloat(FGridCoord A, float B)
{
	return A * B;
}

FGridCoord UGridCoordinateLibraary::Multiply_CoordIntFGridCoord(FGridCoord A, int B)
{
	return A * B;
}
FGridCoord UGridCoordinateLibraary::Multiply_CoordCoord(FGridCoord A, FGridCoord B)
{
	return A * B;
};
FVector2D UGridCoordinateLibraary::Conv_GridCoordToVector2D(FGridCoord A)
{
	return FVector2D(A.Row, A.Col);
}

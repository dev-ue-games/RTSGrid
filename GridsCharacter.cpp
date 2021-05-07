// Fill out your copyright notice in the Description page of Project Settings.


#include "GridsCharacter.h"

// Sets default values
AGridsCharacter::AGridsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGridsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacebleBase.generated.h"

UCLASS()
class RTSGRIDS_API APlacebleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlacebleBase();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Placeable")
		void OnPlacementCompleted();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Placeable")
		void OnPlacemendBegin();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Placeable")
		void OnPlacementCanceled();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placable")
		TSubclassOf<AActor> ConstructionProxy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placable")
		TSubclassOf<AActor> PlacementProxy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placeable")
		float BuildDuration = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placeable")
		float BuildDurationMultiply = 1.0;








protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	AActor* PlacementProxyActor;
	AActor* ConstructionProxyActor;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBallSpawn.generated.h"

UCLASS()
class COUCHGAME2024_API APlayerBallSpawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerBallSpawn();


public:
	FVector GetSpawnSpecialForceDiretion();

private:
	UPROPERTY(EditAnywhere)
	FVector SpawnSpecialForceDirection;
	
	
};

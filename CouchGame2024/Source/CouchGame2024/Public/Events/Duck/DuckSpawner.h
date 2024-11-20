// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DuckSpawner.generated.h"

class APowerUpDuck;
class UDuckData;

UCLASS()
class COUCHGAME2024_API ADuckSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADuckSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StartCountdownSpawning();

	UFUNCTION()
	void StopCountdownSpawning();

	UFUNCTION()
	void SpawnDuck();

	UFUNCTION()
	void GetRandomSpawnTime();

	UPROPERTY(EditAnywhere)
	TSubclassOf<APowerUpDuck> DuckClass;

private:
	UPROPERTY(EditAnywhere)
	float MinDuckSpawnTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float MaxDuckSpawnTime = 20.0f;
	
	UPROPERTY()
	float RandomSpawnTime = 0.f;

	UPROPERTY()
	float CurrentTime = 0.f;

	UPROPERTY()
	float StartedTime = 0.f;

	UPROPERTY()
	bool bCanSpawn = false;

	UPROPERTY()
	APowerUpDuck* SpawnedDuck = nullptr;
};
